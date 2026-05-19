#include "Prim.h"
#include <iostream>

void Prim::runMatrix(MatrixGraph& graph) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* key = new int[vertices];
    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        key[i] = 999999;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < vertices - 1; count++) {
        int min = 999999;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < vertices; v++) {
            int weight = graph.getEdge(u, v);

            if (weight > 0 && !visited[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }

    std::cout << "MST Prim:\n";

    int totalWeight = 0;

    for (int i = 1; i < vertices; i++) {
        std::cout << parent[i] << " - " << i << "  waga: " << key[i] << "\n";
        totalWeight += key[i];
    }

    std::cout << "Suma wag: " << totalWeight << "\n";

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
        key[i] = 999999;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < vertices - 1; count++) {
        int min = 999999;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        visited[u] = true;

        LinkedList<Edge>& list = graph.getAdjList(u);

        for (int i = 0; i < list.getSize(); i++) {
            Edge edge = list.get(i);

            int v = edge.getEnd();
            int weight = edge.getWeight();

            if (!visited[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
        }
    }

    std::cout << "MST Prim:\n";

    int totalWeight = 0;

    for (int i = 1; i < vertices; i++) {
        std::cout << parent[i] << " - " << i << "  waga: " << key[i] << "\n";
        totalWeight += key[i];
    }

    std::cout << "Suma wag: " << totalWeight << "\n";

    delete[] visited;
    delete[] key;
    delete[] parent;
}