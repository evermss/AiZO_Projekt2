#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <limits>

#include "../structures/MatrixGraph.h"
#include "../structures/ListGraph.h"

class Dijkstra {
private:
    static void printPath(int* previous, int vertex) {
        if (vertex == -1) {
            return;
        }

        printPath(previous, previous[vertex]);
        std::cout << vertex << " ";
    }

public:
    template <typename GraphType>
    static void run(
        const GraphType& graph,
        int startVertex,
        int endVertex,
        bool printResult = true
    ) {
        int vertices = graph.getVertices();

        if (vertices <= 0) {
            if (printResult) {
                std::cout << "Graf jest pusty.\n";
            }

            return;
        }

        if (startVertex < 0 || startVertex >= vertices ||
            endVertex < 0 || endVertex >= vertices) {

            if (printResult) {
                std::cout << "Niepoprawny wierzcholek startowy lub koncowy.\n";
            }

            return;
        }

        const int infinity = std::numeric_limits<int>::max() / 4;

        int* distance = new int[vertices];
        int* previous = new int[vertices];
        bool* visited = new bool[vertices];

        for (int i = 0; i < vertices; i++) {
            distance[i] = infinity;
            previous[i] = -1;
            visited[i] = false;
        }

        distance[startVertex] = 0;

        for (int count = 0; count < vertices; count++) {
            int currentVertex = -1;
            int bestDistance = infinity;

            for (int vertex = 0; vertex < vertices; vertex++) {
                if (!visited[vertex] && distance[vertex] < bestDistance) {
                    bestDistance = distance[vertex];
                    currentVertex = vertex;
                }
            }

            if (currentVertex == -1) {
                break;
            }

            if (currentVertex == endVertex) {
                break;
            }

            visited[currentVertex] = true;

            graph.forEachOutgoingEdge(
                currentVertex,
                [&](int neighbour, int weight) {
                    if (neighbour < 0 || neighbour >= vertices) {
                        return;
                    }

                    if (visited[neighbour]) {
                        return;
                    }

                    if (weight < 0) {
                        return;
                    }

                    if (distance[currentVertex] + weight < distance[neighbour]) {
                        distance[neighbour] = distance[currentVertex] + weight;
                        previous[neighbour] = currentVertex;
                    }
                }
            );
        }

        if (printResult) {
            std::cout << "Dijkstra shortest path:\n";

            if (distance[endVertex] == infinity) {
                std::cout << "Brak sciezki z "
                          << startVertex
                          << " do "
                          << endVertex
                          << ".\n";
            } else {
                std::cout << "path = ";
                printPath(previous, endVertex);
                std::cout << "\n";
                std::cout << "cost = " << distance[endVertex] << "\n";
            }
        }

        delete[] distance;
        delete[] previous;
        delete[] visited;
    }

    static void runMatrix(
        const MatrixGraph& graph,
        int startVertex,
        int endVertex,
        bool printResult = true
    );

    static void runList(
        const ListGraph& graph,
        int startVertex,
        int endVertex,
        bool printResult = true
    );
};

#endif