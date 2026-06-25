#ifndef FORDFULKERSON_H
#define FORDFULKERSON_H

#include <iostream>
#include <limits>

#include "../structures/MatrixGraph.h"
#include "../structures/ListGraph.h"

class FordFulkerson {
private:
    static bool bfs(
        int** residualCapacity,
        int vertices,
        int source,
        int sink,
        int* parent
    ) {
        bool* visited = new bool[vertices];
        int* queue = new int[vertices];

        for (int i = 0; i < vertices; i++) {
            visited[i] = false;
            parent[i] = -1;
        }

        int front = 0;
        int back = 0;

        queue[back++] = source;
        visited[source] = true;

        while (front < back) {
            int currentVertex = queue[front++];

            for (int neighbour = 0; neighbour < vertices; neighbour++) {
                if (!visited[neighbour] &&
                    residualCapacity[currentVertex][neighbour] > 0) {

                    parent[neighbour] = currentVertex;
                    visited[neighbour] = true;

                    if (neighbour == sink) {
                        delete[] visited;
                        delete[] queue;
                        return true;
                    }

                    queue[back++] = neighbour;
                }
            }
        }

        delete[] visited;
        delete[] queue;

        return false;
    }

public:
    template <typename GraphType>
    static void run(
        const GraphType& graph,
        int source,
        int sink,
        bool printResult = true
    ) {
        int vertices = graph.getVertices();

        if (vertices <= 0) {
            if (printResult) {
                std::cout << "Graf jest pusty.\n";
            }

            return;
        }

        if (source < 0 || source >= vertices ||
            sink < 0 || sink >= vertices) {

            if (printResult) {
                std::cout << "Niepoprawne zrodlo lub ujscie.\n";
            }

            return;
        }

        if (source == sink) {
            if (printResult) {
                std::cout << "Ford-Fulkerson max flow:\n";
                std::cout << "Max flow: 0\n";
            }

            return;
        }

        int** residualCapacity = new int*[vertices];

        for (int i = 0; i < vertices; i++) {
            residualCapacity[i] = new int[vertices];

            for (int j = 0; j < vertices; j++) {
                residualCapacity[i][j] = 0;
            }
        }

        graph.forEachEdge(
            [&](int start, int end, int capacity) {
                if (start < 0 || start >= vertices ||
                    end < 0 || end >= vertices) {

                    return;
                }

                if (capacity <= 0) {
                    return;
                }

                residualCapacity[start][end] += capacity;
            }
        );

        int* parent = new int[vertices];
        int maxFlow = 0;

        while (bfs(residualCapacity, vertices, source, sink, parent)) {
            int pathFlow = std::numeric_limits<int>::max();

            for (int vertex = sink;
                 vertex != source;
                 vertex = parent[vertex]) {

                int previousVertex = parent[vertex];

                if (residualCapacity[previousVertex][vertex] < pathFlow) {
                    pathFlow = residualCapacity[previousVertex][vertex];
                }
            }

            for (int vertex = sink;
                 vertex != source;
                 vertex = parent[vertex]) {

                int previousVertex = parent[vertex];

                residualCapacity[previousVertex][vertex] -= pathFlow;
                residualCapacity[vertex][previousVertex] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        if (printResult) {
            std::cout << "Ford-Fulkerson max flow:\n";
            std::cout << "Max flow: " << maxFlow << "\n";
        }

        delete[] parent;

        for (int i = 0; i < vertices; i++) {
            delete[] residualCapacity[i];
        }

        delete[] residualCapacity;
    }

    static void runMatrix(
        const MatrixGraph& graph,
        int source,
        int sink,
        bool printResult = true
    );

    static void runList(
        const ListGraph& graph,
        int source,
        int sink,
        bool printResult = true
    );
};

#endif