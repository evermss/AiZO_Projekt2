#ifndef PRIM_H
#define PRIM_H

#include <iostream>
#include <limits>

#include "../structures/MatrixGraph.h"
#include "../structures/ListGraph.h"

class Prim {
public:
    template <typename GraphType>
    static void run(const GraphType& graph, bool printResult = true) {
        int vertices = graph.getVertices();

        if (vertices <= 0) {
            std::cout << "Graf jest pusty.\n";
            return;
        }

        int* parent = new int[vertices];
        int* key = new int[vertices];
        bool* inMst = new bool[vertices];

        for (int i = 0; i < vertices; i++) {
            parent[i] = -1;
            key[i] = std::numeric_limits<int>::max();
            inMst[i] = false;
        }

        key[0] = 0;

        for (int count = 0; count < vertices; count++) {
            int currentVertex = -1;
            int bestKey = std::numeric_limits<int>::max();

            for (int vertex = 0; vertex < vertices; vertex++) {
                if (!inMst[vertex] && key[vertex] < bestKey) {
                    bestKey = key[vertex];
                    currentVertex = vertex;
                }
            }

            if (currentVertex == -1) {
                break;
            }

            inMst[currentVertex] = true;

            graph.forEachIncidentEdge(
                currentVertex,
                [&](int neighbour, int weight) {
                    if (neighbour >= 0 &&
                        neighbour < vertices &&
                        !inMst[neighbour] &&
                        weight < key[neighbour]) {

                        parent[neighbour] = currentVertex;
                        key[neighbour] = weight;
                    }
                }
            );
        }

        long long totalWeight = 0;

        if (printResult) {
            std::cout << "Prim MST:\n";
        }

        for (int vertex = 1; vertex < vertices; vertex++) {
            if (parent[vertex] != -1) {
                if (printResult) {
                    std::cout << parent[vertex]
                              << " - "
                              << vertex
                              << " : "
                              << key[vertex]
                              << "\n";
                }

                totalWeight += key[vertex];
            }
        }

        if (printResult) {
            std::cout << "Suma wag: " << totalWeight << "\n";
        }

        delete[] parent;
        delete[] key;
        delete[] inMst;
    }

    static void runMatrix(const MatrixGraph& graph, bool printResult = true);
    static void runList(const ListGraph& graph, bool printResult = true);
};

#endif