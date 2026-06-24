#include "BellmanFord.h"
#include "Parameters.h"
#include <iostream>

static void printBellmanFordPath(int* previous, int vertex) {
    if (vertex == -1) {
        return;
    }

    if (previous[vertex] != -1) {
        printBellmanFordPath(previous, previous[vertex]);
        std::cout << " -> ";
    }

    std::cout << vertex;
}

void BellmanFord::runMatrix(MatrixGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    int* distance = new int[vertices];
    int* previous = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distance[i] = 999999;
        previous[i] = -1;
    }

    distance[startVertex] = 0;

    for (int k = 0; k < vertices - 1; k++) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int weight = graph.getEdge(i, j);

                if (weight > 0 &&
                    distance[i] != 999999 &&
                    distance[i] + weight < distance[j]) {

                    distance[j] = distance[i] + weight;
                    previous[j] = i;
                }
            }
        }
    }

    int endVertex = Parameters::vertexEnd;

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    std::cout << "Bellman-Ford:\n";
    std::cout << "Start vertex: " << startVertex << "\n";
    std::cout << "End vertex: " << endVertex << "\n";
    std::cout << "Shortest path result:\n";

    if (distance[endVertex] == 999999) {
        std::cout << "brak sciezki\n";
    } else {
        std::cout << "cost = " << distance[endVertex] << ", path = ";
        printBellmanFordPath(previous, endVertex);
        std::cout << "\n";
    }

    delete[] distance;
    delete[] previous;
}

void BellmanFord::runList(ListGraph& graph, int startVertex) {
    int vertices = graph.getVertices();

    int* distance = new int[vertices];
    int* previous = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distance[i] = 999999;
        previous[i] = -1;
    }

    distance[startVertex] = 0;

    for (int k = 0; k < vertices - 1; k++) {
        for (int i = 0; i < vertices; i++) {
            LinkedList<Edge>& list = graph.getAdjList(i);

            for (int j = 0; j < list.getSize(); j++) {
                Edge edge = list.get(j);

                int start = edge.getStart();
                int end = edge.getEnd();
                int weight = edge.getWeight();

                if (distance[start] != 999999 &&
                    distance[start] + weight < distance[end]) {

                    distance[end] = distance[start] + weight;
                    previous[end] = start;
                }
            }
        }
    }

    int endVertex = Parameters::vertexEnd;

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    std::cout << "Bellman-Ford:\n";
    std::cout << "Start vertex: " << startVertex << "\n";
    std::cout << "End vertex: " << endVertex << "\n";
    std::cout << "Shortest path result:\n";

    if (distance[endVertex] == 999999) {
        std::cout << "brak sciezki\n";
    } else {
        std::cout << "cost = " << distance[endVertex] << ", path = ";
        printBellmanFordPath(previous, endVertex);
        std::cout << "\n";
    }

    delete[] distance;
    delete[] previous;
}