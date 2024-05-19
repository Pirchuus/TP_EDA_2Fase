#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma region Graph
/// <summary>
/// Struct to represent a node in a graph
/// </summary>
typedef struct node
{
    int id;
    int value;
    int numAdj;
    struct node** adjacents;
} Node;

/// <summary>
/// Struct to represent a graph
/// </summary>
typedef struct
{
    Node** vertices;
    int numVertices;
    int size;
} Graph;
#pragma endregion

#pragma region Vertex
/// <summary>
/// Function to add a vertex to a graph
/// </summary>
/// <param name="g"></param>
/// <param name="value"></param>
void addVertex(Graph* g, int value)
{
    // If the number of vertices is equal to the size of the array, reallocate memory
    if (g->numVertices == g->size)
    {
        // Double the size of the array
        g->size *= 2;
        // Reallocate memory for the array of vertices
        Node** temp = realloc(g->vertices, g->size * sizeof(Node*));

        // Check if memory reallocation was successful
        if (!temp)
        {
            perror("Failed to reallocate memory for vertices");
            exit(EXIT_FAILURE);
        }
        g->vertices = temp;
    }

    // Create a new node
    Node* newNode = malloc(sizeof(Node));

    // Check if memory allocation was successful
    if (!newNode)
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    newNode->id = g->numVertices;
    newNode->value = value;
    newNode->adjacents = NULL;
    newNode->numAdj = 0;
    g->vertices[g->numVertices++] = newNode;
}

/// <summary>
/// Function to remove a vertex from a graph
/// </summary>
/// <param name="g"></param>
/// <param name="vertexIndex"></param>
void removeVertex(Graph* g, int vertexIndex)
{
    // Check if the vertex Index's are valid
    if (vertexIndex < 0 || vertexIndex >= g->numVertices)
    {
        printf("Invalid vertex index.\n");
        return;
    }

    // Get the vertex to be removed
    Node* vertexToRemove = g->vertices[vertexIndex];

    // First, remove all edges from other vertices pointing to this vertex
    for (int i = 0; i < g->numVertices; i++)
    {
        if (i == vertexIndex) continue; // Skip the vertex being removed
        Node* vertex = g->vertices[i];

        int j = 0;
        while (j < vertex->numAdj) {
            if (vertex->adjacents[j]->id == vertexToRemove->id)
            {
                // Shift all elements to the left to overwrite the current position
                for (int k = j; k < vertex->numAdj - 1; k++)
                {
                    vertex->adjacents[k] = vertex->adjacents[k + 1];
                }
                vertex->numAdj--; // Decrease the count of adjacents
            }
            else {
                j++; // Only increment if no deletion was made to keep the index correct
            }
        }
    }

    // Free the memory allocated for the adjacent vertices of the vertex to be removed
    free(vertexToRemove->adjacents);

    // Free the memory allocated for the vertex itself
    free(vertexToRemove);

    // Shift all vertices after the removed vertex one position to the left
    for (int i = vertexIndex; i < g->numVertices - 1; i++)
    {
        g->vertices[i] = g->vertices[i + 1];
        g->vertices[i]->id = i; // Update the ID of each vertex to reflect their new position
    }

    g->numVertices--; // Decrease the total number of vertices in the graph
}

/// <summary>
/// Function to add a vertex at the start of the graph
/// </summary>
/// <param name="g"></param>
/// <param name="value"></param>
void addVertexAtStart(Graph* g, int value)
{
    // Check if the number of vertices is equal to the size of the array
    if (g->numVertices == g->size)
    {
        // Double the size of the array
        g->size *= 2;

        // Reallocate memory for the array of vertices
        Node** temp = realloc(g->vertices, g->size * sizeof(Node*));

        // Check if memory reallocation was successful
        if (!temp)
        {
            perror("Failed to reallocate memory for vertices");
            exit(EXIT_FAILURE);
        }
        g->vertices = temp;
    }

    // Shift all vertices one position to the right
    for (int i = g->numVertices; i > 0; i--)
    {
        g->vertices[i] = g->vertices[i - 1];
    }

    // Create a new node for the first position
    Node* newNode = malloc(sizeof(Node));
    if (!newNode)
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    newNode->id = 0; // Assign ID 0 to the new first vertex
    newNode->value = value;
    newNode->adjacents = NULL;
    newNode->numAdj = 0;
    g->vertices[0] = newNode;

    // Update IDs for all vertices
    for (int i = 1; i <= g->numVertices; i++)
    {
        g->vertices[i]->id = i;
    }

    g->numVertices++;
}

/// <summary>
/// Function to add a vertex at the end of the graph
/// </summary>
/// <param name="g"></param>
/// <param name="value"></param>
void addVertexAtEnd(Graph* g, int value) {
    addVertex(g, value);
}

/// <summary>
/// Function to update the value of a vertex in a graph
/// </summary>
/// <param name="g"></param>
/// <param name="vertexId"></param>
/// <param name="newValue"></param>
void updateVertexValue(Graph* g, int vertexId, int newValue)
{
    // Check if the vertex ID is valid
    if (vertexId < 0 || vertexId >= g->numVertices) {
        printf("Invalid vertex ID.\n");
        return;
    }

    // Update the value of the vertex
    Node* vertex = g->vertices[vertexId];
    vertex->value = newValue;
}
#pragma endregion

#pragma region Edge
/// <summary>
/// Function to add an edge to a graph
/// </summary>
/// <param name="g"></param>
/// <param name="from"></param>
/// <param name="to"></param>
void addEdge(Graph* g, int from, int to)
{
    // Check if the vertex Index's are valid
    if (from < 0 || from >= g->numVertices || to < 0 || to >= g->numVertices)
    {
        printf("Invalid vertex index.\n");
        return;
    }

    // Get the source vertex
    Node* src = g->vertices[from];
    Node** temp = realloc(src->adjacents, (src->numAdj + 1) * sizeof(Node*));

    // Check if memory reallocation was successful
    if (!temp)
    {
        perror("Failed to reallocate memory for adjacents");
        exit(EXIT_FAILURE);
    }

    // Update the list of adjacent vertices
    src->adjacents = temp;
    src->adjacents[src->numAdj++] = g->vertices[to];
}

/// <summary>
/// Function to remove an edge from a graph
/// </summary>
/// <param name="g"></param>
/// <param name="from"></param>
/// <param name="to"></param>
void removeEdge(Graph* g, int from, int to)
{
    // Check if the vertex Index's are valid
    if (from < 0 || from >= g->numVertices || to < 0 || to >= g->numVertices)
    {
        printf("Invalid vertex index.\n");
        return;
    }

    Node* src = g->vertices[from]; // Get the source vertex

    for (int i = 0; i < src->numAdj; i++)
    {
        if (src->adjacents[i]->id == to)
        {
            // Found the edge, now remove it by shifting left the elements in the adjacency array
            for (int j = i; j < src->numAdj - 1; j++)
            {
                src->adjacents[j] = src->adjacents[j + 1];
            }

            src->numAdj--; // Decrease the count of adjacents
            src->adjacents = realloc(src->adjacents, src->numAdj * sizeof(Node*)); // Optionally resize the memory

            if (src->adjacents == NULL && src->numAdj > 0)
            {
                perror("Failed to reallocate memory for adjacents");
                exit(EXIT_FAILURE);
            }
            printf("Edge removed successfully from %d to %d.\n", from, to);
            return; // Exit the function after the edge is removed
        }
    }

    printf("No edge found from %d to %d.\n", from, to);
}
#pragma endregion

#pragma region Graph Functions
/// <summary>
/// Function to create a new graph
/// </summary>
/// <param name="initialSize"></param>
/// <returns></returns>
Graph* createGraph(int initialSize)
{
    // Allocate memory for the graph
    Graph* g = malloc(sizeof(Graph));

    // Check if memory allocation was successful
    if (!g)
    {
        perror("Failed to allocate memory for graph");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the array of vertices
    g->vertices = malloc(initialSize * sizeof(Node*));

    // Check if memory allocation was successful
    if (!g->vertices)
    {
        free(g); // Clean up previously allocated graph memory before exiting
        perror("Failed to allocate memory for vertices");
        exit(EXIT_FAILURE);
    }

    g->numVertices = 0;
    g->size = initialSize;

    return g;
}

/// <summary>
/// Function to free the memory allocated for the graph
/// </summary>
/// <param name="g"></param>
void freeGraph(Graph* g)
{
    // Free the memory allocated for each vertex
    for (int i = 0; i < g->numVertices; i++)
    {
        free(g->vertices[i]->adjacents);
        free(g->vertices[i]);
    }

    free(g->vertices);
    free(g);
}

/// <summary>
/// Function to load a graph from a file
/// </summary>
/// <param name="g"></param>
/// <param name="filename"></param>
void loadMatrixFromFile(Graph* g, const char* filename)
{
    // Open the file
    FILE* file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (!file)
    {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line
    char line[1024];
    int vertexId = 0;
    int numRows = 0;
    int numCols = 0;

    // Count the number of columns
    if (fgets(line, sizeof(line), file))
    {
        char* token = strtok(line, ";");

        while (token)
        {
            numCols++;
            token = strtok(NULL, ";");
        }
    }
    // Return to the beginning of the file
    rewind(file);

    // Get the number of rows and add the vertices to the graph
    while (fgets(line, sizeof(line), file))
    {
        numRows++;
        char* token = strtok(line, ";");

        // Add the vertices to the graph
        while (token)
        {
            int value;
            sscanf(token, "%d", &value);
            addVertex(g, value);
            token = strtok(NULL, ";");
        }
    }
    fclose(file);

    // Canno't create a connection between the vertices in diagonal
    // Connect the vertices in the graph
    for (int i = 0; i < g->numVertices; i++)
    {
        int row = i / numCols;
        int col = i % numCols;

        // Connect to the right
        if (col < numCols - 1)
        {
            addEdge(g, i, i + 1);
        }
        // Connect to the bottom
        if (row < numRows - 1)
        {
            addEdge(g, i, i + numCols);
        }
    }
}

/// <summary>
/// Function to generate a DOT file from a graph, highlighting the best path in red
/// </summary>
/// <param name="g"></param>
/// <param name="filename"></param>
void generateDotFile(Graph* g, const char* filename, int bestPath[], int bestPathLen)
{
    // Open the file
    FILE* file = fopen(filename, "w");

    // Check if the file was opened successfully
    if (!file)
    {
        perror("Unable to create file");
        exit(EXIT_FAILURE);
    }

    // Write the graph to the file
    fprintf(file, "digraph G {\n");

    // Output all vertices
    for (int i = 0; i < g->numVertices; i++)
    {
        fprintf(file, "    %d [label=\"%d\"];\n", g->vertices[i]->id, g->vertices[i]->value);
    }

    // Output all edges with special color for the best path
    for (int i = 0; i < g->numVertices; i++)
    {
        for (int j = 0; j < g->vertices[i]->numAdj; j++)
        {
            int isBestPath = 0;
            for (int k = 0; k < bestPathLen - 1; k++)
            {
                if (bestPath[k] == i && bestPath[k + 1] == g->vertices[i]->adjacents[j]->id)
                {
                    isBestPath = 1;
                    break;
                }
            }
            if (isBestPath)
                fprintf(file, "    %d -> %d [color=red];\n", i, g->vertices[i]->adjacents[j]->id);
            else
                fprintf(file, "    %d -> %d;\n", i, g->vertices[i]->adjacents[j]->id);
        }
    }

    fprintf(file, "}\n");
    fclose(file);
}

/// <summary>
/// Function to print the graph to the console
/// </summary>
/// <param name="g">Pointer to the graph</param>
void printGraph(Graph* g)
{
    // Print each vertex and its adjacent vertices
    for (int i = 0; i < g->numVertices; i++)
    {
        // Get the vertex
        Node* vertex = g->vertices[i];
        printf("Vertex %d (Value %d): ", vertex->id + 1, vertex->value);

        // Print the adjacent vertices
        if (vertex->numAdj > 0)
        {
            printf("-> ");
            for (int j = 0; j < vertex->numAdj; j++)
            {
                Node* adjVertex = vertex->adjacents[j];
                printf("%d (Value %d)", adjVertex->id + 1, adjVertex->value);
                if (j < vertex->numAdj - 1)
                    printf(", ");
            }
        }
        else
        {
            printf("No connections");
        }
        printf("\n");
    }
}
#pragma endregion

#pragma region DFS
/// <summary>
/// Function to backtrack through the graph using DFS (Depth First Search)
/// </summary>
/// <param name="g"></param>
/// <param name="v"></param>
/// <param name="visited"></param>
/// <param name="path"></param>
/// <param name="pathIndex"></param>
/// <param name="maxSum"></param>
/// <param name="currentSum"></param>
/// <param name="bestPath"></param>
/// <param name="bestPathLen"></param>
void dfsBacktraking(Graph* g, int v, int visited[], int path[], int* pathIndex, int* maxSum, int currentSum, int bestPath[], int* bestPathLen)
{
    // Mark the current vertex as visited and add it to the path
    visited[v] = 1;
    path[(*pathIndex)++] = v;
    currentSum += g->vertices[v]->value;

    // If the current sum is greater than the maximum sum, update the maximum sum
    if (currentSum > *maxSum)
    {
        *maxSum = currentSum;
        *bestPathLen = *pathIndex;
        memcpy(bestPath, path, (*pathIndex) * sizeof(int));
    }

    // Recursively visit the adjacent vertices
    for (int i = 0; i < g->vertices[v]->numAdj; i++)
    {
        int adj = g->vertices[v]->adjacents[i]->id;
        if (!visited[adj]) {
            dfsBacktraking(g, adj, visited, path, pathIndex, maxSum, currentSum, bestPath, bestPathLen);
        }
    }

    // Backtrack
    visited[v] = 0;
    (*pathIndex)--;
}

/// <summary>
/// Function to find the highest sum path in a graph using DFS (Depth First Search)
/// </summary>
/// <param name="g"></param>
/// <param name="startVertex"></param>
/// <param name="maxSum"></param>
/// <param name="bestPath"></param>
/// <param name="bestPathLen"></param>
void dfs(Graph* g, int startVertex, int* maxSum, int bestPath[], int* bestPathLen)
{
    // Create an array to keep track of visited vertices
    int* visited = calloc(g->numVertices, sizeof(int));
    int* path = malloc(g->numVertices * sizeof(int));
    int pathIndex = 0;
    *maxSum = 0;
    *bestPathLen = 0;

    // Start the DFS from the given vertex
    dfsBacktraking(g, startVertex, visited, path, &pathIndex, maxSum, 0, bestPath, bestPathLen);

    // Free the memory allocated for the visited array
    free(visited);
    free(path);
}

/// <summary>
/// Function to perform DFS and store all paths from source to destination
/// </summary>
/// <param name="g"></param>
/// <param name="v"></param>
/// <param name="dest"></param>
/// <param name="visited"></param>
/// <param name="path"></param>
/// <param name="pathIndex"></param>
void allPathsDFS(Graph* g, int v, int dest, int visited[], int path[], int pathIndex, int currentSum)
{
    // Mark the current vertex as visited and add it to the path
    visited[v] = 1;
    path[pathIndex++] = g->vertices[v]->value;
    currentSum += g->vertices[v]->value;

    // If the destination vertex is reached, print the path
    if (v == dest)
    {
        // Print the path
        for (int i = 0; i < pathIndex; i++)
        {
            printf("%d -> ", path[i]);
        }
        printf("(Soma: %d)\n", currentSum);
    }
    // Otherwise, recursively visit the adjacent vertices
    else
    {
        // Recursively visit the adjacent vertices
        for (int i = 0; i < g->vertices[v]->numAdj; i++)
        {
            // Get the adjacent vertex
            int adj = g->vertices[v]->adjacents[i]->id;
            if (!visited[adj])
            {
                // Recursively visit the adjacent vertex
                allPathsDFS(g, adj, dest, visited, path, pathIndex, currentSum);
            }
        }
    }

    // Mark the current vertex as unvisited
    visited[v] = 0;
    pathIndex--;
    currentSum -= g->vertices[v]->value;
}

/// <summary>
/// Function to find and print all paths from a source to a destination using DFS
/// </summary>
/// <param name="g"></param>
/// <param name="startVertex"></param>
/// <param name="endVertex"></param>
void allPaths(Graph* g, int startVertex, int endVertex)
{
    int* visited = calloc(g->numVertices, sizeof(int));
    int* path = malloc(g->numVertices * sizeof(int));
    int pathIndex = 0;
    int currentSum = 0;

    // Print all paths from the start vertex to the end vertex
    printf("All paths from %d to %d:\n", startVertex + 1, endVertex + 1);
    allPathsDFS(g, startVertex, endVertex, visited, path, pathIndex, currentSum);

    // Free the memory allocated for the visited array
    free(visited);
    free(path);
}
#pragma endregion

#pragma region Main
/// <summary>
/// Main function
/// </summary>
/// <returns></returns>
int main()
{
    // Create a graph
    Graph* graph = createGraph(1);
    int choice = 0, choice2 = 0, newValue, index, from, to, maxSum = 0, bestPathLen = 0;
    int* bestPath = malloc(graph->numVertices * sizeof(int));

    loadMatrixFromFile(graph, "Matrix.txt");
    generateDotFile(graph, "Graph.dot", NULL, 0);
    system("dot -Tpng Graph.dot -o Graph.png");
    system("start Graph.png");

    do
    {
        printf("\nMenu:\n");
        printf("1. Update Vertex\n");
        printf("2. Add Vertex\n");
        printf("3. Add Edge\n");
        printf("4. Remove Vertex\n");
        printf("5. Remove Edge\n");
        printf("6. All Paths\n");
        printf("7. Highest Sum\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            printf("Graph:\n");
            printGraph(graph);
            printf("\n\n");
            printf("Vertex: ");
            scanf("%d", &index);
            printf("New Value: ");
            scanf("%d", &newValue);
            updateVertexValue(graph, index - 1, newValue);
            printf("\nValue updated successfully.\n\n");
            printGraph(graph);
            break;

        case 2:
            do
            {
                printf("\n\nAdd Vertex:\n");
                printf("1. At Start\n");
                printf("2. At End\n");
                printf("0. Back\n");
                printf("Enter your choice for adding a vertex: ");
                scanf("%d", &choice2);

                switch (choice2)
                {
                case 1:
                    system("cls");
                    printf("Add Vertex at Start:\n");
                    printf("Vertex Value: ");
                    scanf("%d", &newValue);
                    addVertexAtStart(graph, newValue);
                    printf("\nVertex added successfully.\n\n");
                    printGraph(graph);
                    break;

                case 2:
                    system("cls");
                    printf("Add Vertex at End:\n");
                    printf("Vertex Value: ");
                    scanf("%d", &newValue);
                    addVertexAtEnd(graph, newValue);
                    printf("\nVertex added successfully.\n\n");
                    printGraph(graph);
                    break;

                case 0:
                    system("cls");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
                }
            } while (choice2 != 0);
            break;

        case 3:
            system("cls");
            printf("Graph:\n");
            printGraph(graph);
            printf("\nAdd an Edge:\n");
            printf("From Vertex Index: ");
            scanf("%d", &from);
            printf("To Vertex Index: ");
            scanf("%d", &to);
            addEdge(graph, from - 1, to - 1);
            system("cls");
            printGraph(graph);
            printf("\nEdge added successfully from %d to %d.\n\n", from, to);
            break;

        case 4:
            system("cls");
            printf("Graph:\n");
            printGraph(graph);
            printf("\n\n");
            printf("Vertex to be removed: ");
            scanf("%d", &index);
            removeVertex(graph, index - 1);
            printf("\nVertex removed successfully.\n\n");
            printGraph(graph);
            break;

        case 5:
            system("cls");
            printf("Graph:\n");
            printGraph(graph);
            printf("\nRemove an Edge:\n");
            printf("From Vertex Index: ");
            scanf("%d", &from);
            printf("To Vertex Index: ");
            scanf("%d", &to);
            removeEdge(graph, from - 1, to - 1);
            printf("\nEdge removed successfully.\n\n");
            printGraph(graph);
            break;

        case 6:
            system("cls");
            allPaths(graph, 0, graph->numVertices - 1);
            break;

        case 7:
            system("cls");
            printf("Graph:\n");
            printGraph(graph);
            printf("\n\n");
            dfs(graph, 0, &maxSum, bestPath, &bestPathLen);

            printf("Highest sum: %d\n", maxSum);
            printf("Path: ");
            for (int i = 0; i < bestPathLen; i++)
            {
                printf("%d ", bestPath[i] + 1);
            }
            printf("\n");
            break;

        case 0:
            system("cls");
            printf("Exiting the program...\n");
            generateDotFile(graph, "GraphAndPath.dot", bestPath, bestPathLen);
            system("dot -Tpng GraphAndPath.dot -o GraphAndPath.png");
            system("start GraphAndPath.png");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Free the memory allocated for the graph
    freeGraph(graph);
    return 0;
}
#pragma endregion