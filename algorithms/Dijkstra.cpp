#include "Dijkstra.h"
#include <iostream>
#include "Parameters.h"

static void printDijkstraPath(int* previous, int vertex) {
    if (vertex == -1) {
        return;
    }

    if (previous[vertex] != -1) {
        printDijkstraPath(previous, previous[vertex]);
        std::cout << " -> ";
    }

    std::cout << vertex;
}

void Dijkstra::runMatrix(MatrixGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* distance = new int[vertices];
    int* previous = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        distance[i] = 999999;
        previous[i] = -1;
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
                distance[u] != 999999 &&
                distance[u] + weight < distance[v]) {

                distance[v] = distance[u] + weight;
                previous[v] = u;
            }
        }
    }

    int endVertex = Parameters::vertexEnd;

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    std::cout << "Start vertex: " << startVertex << "\n";
    std::cout << "End vertex: " << endVertex << "\n";
    std::cout << "Shortest path result:\n";

    if (distance[endVertex] == 999999) {
        std::cout << "brak sciezki\n";
    } else {
        std::cout << "cost = " << distance[endVertex] << ", path = ";
        printDijkstraPath(previous, endVertex);
        std::cout << "\n";
    }

    delete[] visited;
    delete[] distance;
    delete[] previous;
}

void Dijkstra::runList(ListGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    bool* visited = new bool[vertices];
    int* distance = new int[vertices];
    int* previous = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        visited[i] = false;
        distance[i] = 999999;
        previous[i] = -1;
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
                distance[u] != 999999 &&
                distance[u] + weight < distance[v]) {

                distance[v] = distance[u] + weight;
                previous[v] = u;
            }
        }
    }

    int endVertex = Parameters::vertexEnd;

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    std::cout << "Start vertex: " << startVertex << "\n";
    std::cout << "End vertex: " << endVertex << "\n";
    std::cout << "Shortest path result:\n";

    if (distance[endVertex] == 999999) {
        std::cout << "brak sciezki\n";
    } else {
        std::cout << "cost = " << distance[endVertex] << ", path = ";
        printDijkstraPath(previous, endVertex);
        std::cout << "\n";
    }

    delete[] visited;
    delete[] distance;
    delete[] previous;
}