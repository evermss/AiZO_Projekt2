#include "BellmanFord.h"
#include <iostream>

void BellmanFord::runMatrix(MatrixGraph& graph, int startVertex) {
    int vertices = graph.getVertices();
    int* distance = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distance[i] = 999999;
    }

    distance[startVertex] = 0;

    for (int k = 0; k < vertices - 1; k++) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int weight = graph.getEdge(i, j);

                if (weight > 0 && distance[i] != 999999 &&
                    distance[i] + weight < distance[j]) {
                    distance[j] = distance[i] + weight;
                    }
            }
        }
    }

    std::cout << "Bellman-Ford:\n";

    for (int i = 0; i < vertices; i++) {
        std::cout << startVertex << " -> " << i << " = " << distance[i] << "\n";
    }

    delete[] distance;
}

void BellmanFord::runList(ListGraph& graph, int startVertex) {
    int vertices = graph.getVertices();
    int* distance = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        distance[i] = 999999;
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
                    }
            }
        }
    }

    std::cout << "Bellman-Ford:\n";

    for (int i = 0; i < vertices; i++) {
        std::cout << startVertex << " -> " << i << " = " << distance[i] << "\n";
    }

    delete[] distance;
}