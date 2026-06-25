#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include <iostream>
#include <limits>

#include "../structures/MatrixGraph.h"
#include "../structures/ListGraph.h"

class BellmanFord {
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

        const long long infinity =
            std::numeric_limits<long long>::max() / 4;

        long long* distance = new long long[vertices];
        int* previous = new int[vertices];

        for (int i = 0; i < vertices; i++) {
            distance[i] = infinity;
            previous[i] = -1;
        }

        distance[startVertex] = 0;

        for (int iteration = 0; iteration < vertices - 1; iteration++) {
            bool changed = false;

            graph.forEachEdge(
                [&](int start, int end, int weight) {
                    if (start < 0 || start >= vertices ||
                        end < 0 || end >= vertices) {

                        return;
                    }

                    if (distance[start] == infinity) {
                        return;
                    }

                    long long newDistance = distance[start] + weight;

                    if (newDistance < distance[end]) {
                        distance[end] = newDistance;
                        previous[end] = start;
                        changed = true;
                    }
                }
            );

            if (!changed) {
                break;
            }
        }

        bool hasNegativeCycle = false;

        graph.forEachEdge(
            [&](int start, int end, int weight) {
                if (start < 0 || start >= vertices ||
                    end < 0 || end >= vertices) {

                    return;
                }

                if (distance[start] == infinity) {
                    return;
                }

                if (distance[start] + weight < distance[end]) {
                    hasNegativeCycle = true;
                }
            }
        );

        if (printResult) {
            std::cout << "Bellman-Ford shortest path:\n";

            if (hasNegativeCycle) {
                std::cout << "Wykryto cykl o ujemnej wadze.\n";
            } else if (distance[endVertex] == infinity) {
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