#include "FordFulkerson.h"
#include <iostream>
#include <climits>

void FordFulkerson::runMatrix(MatrixGraph& graph, int source, int sink) {
    int vertices = graph.getVertices();

    if (
        source < 0 ||
        source >= vertices ||
        sink < 0 ||
        sink >= vertices
    ) {
        std::cout << "Blad: niepoprawne zrodlo lub ujscie.\n";
        return;
    }

    if (source == sink) {
        std::cout << "Blad: zrodlo i ujscie musza byc rozne.\n";
        return;
    }

    int** residual = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        residual[i] = new int[vertices];

        for (int j = 0; j < vertices; j++) {
            residual[i][j] = graph.getEdge(i, j);
        }
    }

    int maxFlow = 0;

    while (true) {

        bool* visited = new bool[vertices];
        int* parent = new int[vertices];

        for (int i = 0; i < vertices; i++) {
            visited[i] = false;
            parent[i] = -1;
        }

        int* queue = new int[vertices];
        int front = 0;
        int back = 0;

        queue[back++] = source;
        visited[source] = true;

        while (front < back) {

            int u = queue[front++];

            for (int v = 0; v < vertices; v++) {

                if (!visited[v] && residual[u][v] > 0) {
                    queue[back++] = v;
                    visited[v] = true;
                    parent[v] = u;
                }
            }
        }

        if (!visited[sink]) {
            delete[] visited;
            delete[] parent;
            delete[] queue;
            break;
        }

        int pathFlow = 999999;

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            if (residual[u][v] < pathFlow) {
                pathFlow = residual[u][v];
            }
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;

        delete[] queue;
        delete[] visited;
        delete[] parent;
    }

    std::cout << "Max flow: " << maxFlow << "\n";

    for (int i = 0; i < vertices; i++) {
        delete[] residual[i];
    }

    delete[] residual;
}

void FordFulkerson::runList(ListGraph& graph, int source, int sink) {
    int vertices = graph.getVertices();

    if (
        source < 0 ||
        source >= vertices ||
        sink < 0 ||
        sink >= vertices
    ) {
        std::cout << "Blad: niepoprawne zrodlo lub ujscie.\n";
        return;
    }

    if (source == sink) {
        std::cout << "Blad: zrodlo i ujscie musza byc rozne.\n";
        return;
    }

    int** residual = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        residual[i] = new int[vertices];

        for (int j = 0; j < vertices; j++) {
            residual[i][j] = 0;
        }
    }

    for (int i = 0; i < vertices; i++) {
        LinkedList<Edge>& list = graph.getAdjList(i);

        for (int j = 0; j < list.getSize(); j++) {
            Edge edge = list.get(j);
            residual[edge.getStart()][edge.getEnd()] = edge.getWeight();
        }
    }

    int maxFlow = 0;

    while (true) {
        bool* visited = new bool[vertices];
        int* parent = new int[vertices];

        for (int i = 0; i < vertices; i++) {
            visited[i] = false;
            parent[i] = -1;
        }

        int* queue = new int[vertices];
        int front = 0;
        int back = 0;

        queue[back++] = source;
        visited[source] = true;

        while (front < back) {
            int u = queue[front++];

            for (int v = 0; v < vertices; v++) {
                if (!visited[v] && residual[u][v] > 0) {
                    queue[back++] = v;
                    visited[v] = true;
                    parent[v] = u;
                }
            }
        }

        if (!visited[sink]) {
            delete[] visited;
            delete[] parent;
            delete[] queue;
            break;
        }

        int pathFlow = 999999;

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            if (residual[u][v] < pathFlow) {
                pathFlow = residual[u][v];
            }
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;

        delete[] queue;
        delete[] visited;
        delete[] parent;
    }

    std::cout << "Max flow: " << maxFlow << "\n";

    for (int i = 0; i < vertices; i++) {
        delete[] residual[i];
    }

    delete[] residual;
}