#include "Kruskal.h"
#include <iostream>

void Kruskal::runMatrix(MatrixGraph& graph) {
    int vertices = graph.getVertices();

    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    auto find = [&](int v) {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    };

    std::cout << "MST Kruskal:\n";

    int edgesAdded = 0;
    int totalWeight = 0;

    while (edgesAdded < vertices - 1) {
        int minWeight = 999999;
        int start = -1;
        int end = -1;

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int weight = graph.getEdge(i, j);

                if (weight > 0 && weight < minWeight) {
                    int root1 = find(i);
                    int root2 = find(j);

                    if (root1 != root2) {
                        minWeight = weight;
                        start = i;
                        end = j;
                    }
                }
            }
        }

        if (start == -1 || end == -1) {
            break;
        }

        int root1 = find(start);
        int root2 = find(end);

        parent[root1] = root2;

        std::cout << start << " - " << end << "  waga: " << minWeight << "\n";

        totalWeight += minWeight;
        edgesAdded++;
    }

    std::cout << "Suma wag: " << totalWeight << "\n";

    delete[] parent;
}

void Kruskal::runList(ListGraph& graph) {
    int vertices = graph.getVertices();

    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    auto find = [&](int v) {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    };

    std::cout << "MST Kruskal:\n";

    int edgesAdded = 0;
    int totalWeight = 0;

    while (edgesAdded < vertices - 1) {
        int minWeight = 999999;
        int start = -1;
        int end = -1;

        for (int i = 0; i < vertices; i++) {
            LinkedList<Edge>& list = graph.getAdjList(i);

            for (int j = 0; j < list.getSize(); j++) {
                Edge edge = list.get(j);

                int edgeStart = edge.getStart();
                int edgeEnd = edge.getEnd();
                int weight = edge.getWeight();

                if (weight < minWeight) {
                    int root1 = find(edgeStart);
                    int root2 = find(edgeEnd);

                    if (root1 != root2) {
                        minWeight = weight;
                        start = edgeStart;
                        end = edgeEnd;
                    }
                }
            }
        }

        if (start == -1 || end == -1) {
            break;
        }

        int root1 = find(start);
        int root2 = find(end);

        parent[root1] = root2;

        std::cout << start << " - " << end << "  waga: " << minWeight << "\n";

        totalWeight += minWeight;
        edgesAdded++;
    }

    std::cout << "Suma wag: " << totalWeight << "\n";

    delete[] parent;

}
int Kruskal::getMstWeightMatrix(MatrixGraph& graph) {
    int vertices = graph.getVertices();

    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    auto find = [&](int v) {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    };

    int edgesAdded = 0;
    int totalWeight = 0;

    while (edgesAdded < vertices - 1) {
        int minWeight = 999999;
        int start = -1;
        int end = -1;

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int weight = graph.getEdge(i, j);

                if (weight > 0 && weight < minWeight) {
                    int root1 = find(i);
                    int root2 = find(j);

                    if (root1 != root2) {
                        minWeight = weight;
                        start = i;
                        end = j;
                    }
                }
            }
        }

        if (start == -1 || end == -1) {
            break;
        }

        int root1 = find(start);
        int root2 = find(end);

        parent[root1] = root2;

        totalWeight += minWeight;
        edgesAdded++;
    }

    delete[] parent;

    return totalWeight;
}

int Kruskal::getMstWeightList(ListGraph& graph) {
    int vertices = graph.getVertices();

    int* parent = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    auto find = [&](int v) {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    };

    int edgesAdded = 0;
    int totalWeight = 0;

    while (edgesAdded < vertices - 1) {
        int minWeight = 999999;
        int start = -1;
        int end = -1;

        for (int i = 0; i < vertices; i++) {
            LinkedList<Edge>& list = graph.getAdjList(i);

            for (int j = 0; j < list.getSize(); j++) {
                Edge edge = list.get(j);

                int edgeStart = edge.getStart();
                int edgeEnd = edge.getEnd();
                int weight = edge.getWeight();

                if (weight < minWeight) {
                    int root1 = find(edgeStart);
                    int root2 = find(edgeEnd);

                    if (root1 != root2) {
                        minWeight = weight;
                        start = edgeStart;
                        end = edgeEnd;
                    }
                }
            }
        }

        if (start == -1 || end == -1) {
            break;
        }

        int root1 = find(start);
        int root2 = find(end);

        parent[root1] = root2;

        totalWeight += minWeight;
        edgesAdded++;
    }

    delete[] parent;

    return totalWeight;

}
