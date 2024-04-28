#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Graph
typedef struct node
{
    int id;
    int value;
    int numAdj;
    struct node** adjacents;
} Node;

typedef struct
{
    Node** vertices;
    int numVertices;
    int size;
    int directed;
} Graph;
#pragma endregion

#pragma region Vertex
void addVertex(Graph* g, int value)
{
    if (g->numVertices == g->size)
    {
        g->size *= 2;
        Node** temp = realloc(g->vertices, g->size * sizeof(Node*));

        if (!temp)
        {
            perror("Failed to reallocate memory for vertices");
            exit(EXIT_FAILURE);
        }
        g->vertices = temp;
    }

    Node* newNode = malloc(sizeof(Node));

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

void removeVertx(Graph* g, int vertexIndex)
{
    Node* vertex = g->vertices[vertexIndex];

    for (int i = 0; i < vertex->numAdj; i++)
    {
        Node* adjacent = vertex->adjacents[i];

        for (int j = 0; j < adjacent->numAdj; j++)
        {
            if (adjacent->adjacents[j] == vertex)
            {
                for (int k = j; k < adjacent->numAdj - 1; k++)
                {
                    adjacent->adjacents[k] = adjacent->adjacents[k + 1];
                }
                adjacent->numAdj--;
                break;
            }
        }
    }

    free(vertex->adjacents);
    free(vertex);

    for (int i = vertexIndex; i < g->numVertices - 1; i++)
    {
        g->vertices[i] = g->vertices[i + 1];
    }
    g->numVertices--;
}
#pragma endregion

#pragma region Edge
void addEdge(Graph* g, int from, int to)
{
    Node* src = g->vertices[from];
    Node** temp = realloc(src->adjacents, (src->numAdj + 1) * sizeof(Node*));

    if (!temp)
    {
        perror("Failed to reallocate memory for adjacents");
        exit(EXIT_FAILURE);
    }

    src->adjacents = temp;
    src->adjacents[src->numAdj++] = g->vertices[to];
}

void removeEdge(Graph* g, int from, int to)
{
    Node* src = g->vertices[from];
    for (int i = 0; i < src->numAdj; i++)
    {
        if (src->adjacents[i]->id == to)
        {
            for (int j = i; j < src->numAdj - 1; j++)
            {
                src->adjacents[j] = src->adjacents[j + 1];
            }
            src->numAdj--;
            break;
        }
    }
}
#pragma endregion

#pragma region GraphFunctions
Graph* createGraph(int size, int directed)
{
    Graph* g = malloc(sizeof(Graph));

    g->vertices = malloc(size * sizeof(Node*));
    g->numVertices = 0;
    g->size = size;
    g->directed = directed;

    return g;
}

void freeGraph(Graph* g)
{
    for (int i = 0; i < g->numVertices; i++)
    {
        free(g->vertices[i]->adjacents);
        free(g->vertices[i]);
    }

    free(g->vertices);
    free(g);
}

void loadMatrixFromFile(Graph* g, const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (!file)
    {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int vertexId = 0;
    int numRows = 0;
    int numCols = 0;

    // Primeiro, determinar o número de colunas
    if (fgets(line, sizeof(line), file))
    {
        char* token = strtok(line, ";");

        while (token)
        {
            numCols++;
            token = strtok(NULL, ";");
        }
    }
    rewind(file);

    // Carregar os vértices
    while (fgets(line, sizeof(line), file))
    {
        numRows++;
        char* token = strtok(line, ";");

        while (token)
        {
            int value;
            sscanf(token, "%d", &value);
            addVertex(g, value);
            token = strtok(NULL, ";");
        }
    }
    fclose(file);

    // Adicionar arestas, excluindo diagonais
    for (int i = 0; i < g->numVertices; i++)
    {
        int row = i / numCols;
        int col = i % numCols;

        // Conexão para a direita
        if (col < numCols - 1)
        {
            addEdge(g, i, i + 1);
        }
        // Conexão para baixo
        if (row < numRows - 1)
        {
            addEdge(g, i, i + numCols);
        }
    }
}
#pragma endregion

void generateDotFile(Graph* g, const char* filename)
{
    FILE* file = fopen(filename, "w");

    if (!file)
    {
        perror("Unable to create file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "digraph G {\n");
    for (int i = 0; i < g->numVertices; i++)
    {
        // Imprime o nó com seu valor. Por exemplo: A [label="5"];
        fprintf(file, "    %d [label=\"%d\"];\n", g->vertices[i]->id, g->vertices[i]->value);
    }

    for (int i = 0; i < g->numVertices; i++)
    {
        for (int j = 0; j < g->vertices[i]->numAdj; j++)
        {
            // Imprime uma aresta do nó i para cada um de seus adjacentes.
            fprintf(file, "    %d -> %d;\n", i, g->vertices[i]->adjacents[j]->id);
        }
    }

    fprintf(file, "}\n");
    fclose(file);
}

#pragma region Main
int main()
{
    Graph* g = createGraph(10, 0);

    loadMatrixFromFile(g, "Matrix.txt");
    generateDotFile(g, "graph.dot");
    freeGraph(g);
    return 0;
}
#pragma endregion