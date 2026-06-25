#include "Prim.h"
#include <iostream>

static const int INF = 999999;

static int getUndirectedMatrixWeight(MatrixGraph& graph, int u, int v) {
    int weightUV = graph.getEdge(u, v);
    int weightVU = graph.getEdge(v, u);

    if (weightUV > 0 && weightVU > 0) {
        return weightUV < weightVU ? weightUV : weightVU;
    }

    if (weightUV > 0) {
        return weightUV;
    }

    return weightVU;
}

static void printPrimResult(int* parent, int* key, int vertices) {
    std::cout << "MST Prim:\n";

    int totalWeight = 0;
    bool connected = true;

    for (int i = 1; i < vertices; i++) {
        if (parent[i] == -1 || key[i] == INF) {
            connected = false;
            break;
        }
    }

    if (!connected) {
        std::cout << "Graf jest niespojny - nie mozna wyznaczyc MST dla wszystkich wierzcholkow.\n";
        return;
    }

    for (int i = 1; i < vertices; i++) {
        std::cout << parent[i] << " - " << i << "  waga: " << key[i] << "\n";
        totalWeight += key[i];
    }

    std::cout << "Suma wag: " << totalWeight << "\n";
}

void Prim::runMatrix(MatrixGraph& graph) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* key = new int[vertices];
    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        key[i] = INF;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < vertices - 1; count++) {
        int min = INF;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < vertices; v++) {
            int weight = getUndirectedMatrixWeight(graph, u, v);

            if (weight > 0 && !visited[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }

    printPrimResult(parent, key, vertices);

    delete[] visited;
    delete[] key;
    delete[] parent;
}

void Prim::runList(ListGraph& graph) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* key = new int[vertices];
    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        key[i] = INF;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < vertices - 1; count++) {
        int min = INF;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        // MST jest dla grafu nieskierowanego, dlatego dla listy sasiedztwa
        // sprawdzamy krawedzie wychodzace z u oraz krawedzie dochodzace do u.
        for (int i = 0; i < vertices; i++) {
            LinkedList<Edge>& list = graph.getAdjList(i);

            for (int j = 0; j < list.getSize(); j++) {
                Edge edge = list.get(j);

                int edgeStart = edge.getStart();
                int edgeEnd = edge.getEnd();
                int weight = edge.getWeight();
                int v = -1;

                if (edgeStart == u) {
                    v = edgeEnd;
                } else if (edgeEnd == u) {
                    v = edgeStart;
                }

                if (v != -1 && !visited[v] && weight < key[v]) {
                    parent[v] = u;
                    key[v] = weight;
                }
            }
        }
    }

    printPrimResult(parent, key, vertices);

    delete[] visited;
    delete[] key;
    delete[] parent;
}