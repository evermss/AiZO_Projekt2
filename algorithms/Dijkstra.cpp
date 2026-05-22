#include "Dijkstra.h"
#include <iostream>

void Dijkstra::runMatrix(MatrixGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* distance = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        distance[i] = 999999;
    }

    distance[startVertex] = 0;

    for (int count = 0; count < vertices - 1; count++) {

        int min = 999999;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && distance[v] < min) {
                min = distance[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < vertices; v++) {

            int weight = graph.getEdge(u, v);

            if (weight > 0 &&
                !visited[v] &&
                distance[u] + weight < distance[v]) {

                distance[v] = distance[u] + weight;
            }
        }
    }

    std::cout << "Dijkstra:\n";

    for (int i = 0; i < vertices; i++) {
        std::cout << startVertex
                  << " -> "
                  << i
                  << " = "
                  << distance[i]
                  << "\n";
    }

    delete[] visited;
    delete[] distance;
}

void Dijkstra::runList(ListGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* distance = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        distance[i] = 999999;
    }

    distance[startVertex] = 0;

    for (int count = 0; count < vertices - 1; count++) {

        int min = 999999;
        int u = -1;

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && distance[v] < min) {
                min = distance[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = true;

        LinkedList<Edge>& list = graph.getAdjList(u);

        for (int i = 0; i < list.getSize(); i++) {

            Edge edge = list.get(i);

            int v = edge.getEnd();
            int weight = edge.getWeight();

            if (!visited[v] &&
                distance[u] + weight < distance[v]) {

                distance[v] = distance[u] + weight;
            }
        }
    }

    std::cout << "Dijkstra:\n";

    for (int i = 0; i < vertices; i++) {
        std::cout << startVertex
                  << " -> "
                  << i
                  << " = "
                  << distance[i]
                  << "\n";
    }

    delete[] visited;
    delete[] distance;
}