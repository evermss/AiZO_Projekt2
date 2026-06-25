#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <iostream>

#include "../structures/MatrixGraph.h"
#include "../structures/ListGraph.h"
#include "../structures/Edge.h"

class Kruskal {
private:
    static int findSet(int* parent, int vertex) {
        if (parent[vertex] != vertex) {
            parent[vertex] = findSet(parent, parent[vertex]);
        }

        return parent[vertex];
    }

    static void unionSets(int* parent, int* rank, int first, int second) {
        int firstRoot = findSet(parent, first);
        int secondRoot = findSet(parent, second);

        if (firstRoot == secondRoot) {
            return;
        }

        if (rank[firstRoot] < rank[secondRoot]) {
            parent[firstRoot] = secondRoot;
        } else if (rank[firstRoot] > rank[secondRoot]) {
            parent[secondRoot] = firstRoot;
        } else {
            parent[secondRoot] = firstRoot;
            rank[firstRoot]++;
        }
    }

    static void sortEdgesByWeight(Edge* edges, int edgesCount) {
        for (int gap = edgesCount / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < edgesCount; i++) {
                Edge temp = edges[i];
                int j = i;

                while (j >= gap &&
                       edges[j - gap].getWeight() > temp.getWeight()) {

                    edges[j] = edges[j - gap];
                    j -= gap;
                }

                edges[j] = temp;
            }
        }
    }

public:
    template <typename GraphType>
    static void run(const GraphType& graph, bool printResult = true) {
        int vertices = graph.getVertices();

        if (vertices <= 0) {
            if (printResult) {
                std::cout << "Graf jest pusty.\n";
            }

            return;
        }

        int edgesCount = 0;

        graph.forEachEdge(
            [&](int start, int end, int weight) {
                (void) start;
                (void) end;
                (void) weight;

                edgesCount++;
            }
        );

        if (edgesCount == 0) {
            if (printResult) {
                std::cout << "Kruskal MST:\n";
                std::cout << "Suma wag: 0\n";
            }

            return;
        }

        Edge* edges = new Edge[edgesCount];
        int currentEdge = 0;

        graph.forEachEdge(
            [&](int start, int end, int weight) {
                edges[currentEdge] = Edge(start, end, weight);
                currentEdge++;
            }
        );

        sortEdgesByWeight(edges, edgesCount);

        int* parent = new int[vertices];
        int* rank = new int[vertices];

        for (int i = 0; i < vertices; i++) {
            parent[i] = i;
            rank[i] = 0;
        }

        long long totalWeight = 0;
        int selectedEdges = 0;

        if (printResult) {
            std::cout << "Kruskal MST:\n";
        }

        for (int i = 0; i < edgesCount && selectedEdges < vertices - 1; i++) {
            int start = edges[i].getStart();
            int end = edges[i].getEnd();
            int weight = edges[i].getWeight();

            if (start < 0 || start >= vertices ||
                end < 0 || end >= vertices) {

                continue;
            }

            int startRoot = findSet(parent, start);
            int endRoot = findSet(parent, end);

            if (startRoot != endRoot) {
                if (printResult) {
                    std::cout << start
                              << " - "
                              << end
                              << " : "
                              << weight
                              << "\n";
                }

                totalWeight += weight;
                selectedEdges++;

                unionSets(parent, rank, startRoot, endRoot);
            }
        }

        if (printResult) {
            std::cout << "Suma wag: " << totalWeight << "\n";
        }

        delete[] edges;
        delete[] parent;
        delete[] rank;
    }

    static void runMatrix(
        const MatrixGraph& graph,
        bool printResult = true
    );

    static void runList(
        const ListGraph& graph,
        bool printResult = true
    );
};

#endif