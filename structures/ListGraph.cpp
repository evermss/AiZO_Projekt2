#include "ListGraph.h"
#include <iostream>

// Konstruktor
ListGraph::ListGraph(int vertices) : vertices(vertices) {
    adjList = new LinkedList<Edge>[vertices];
}

// Destruktor
ListGraph::~ListGraph() {
    delete[] adjList;
}

// Dodanie krawędzi
void ListGraph::addEdge(int start, int end, int weight) {
    adjList[start].add(Edge(start, end, weight));
}

// Wyświetlanie grafu
void ListGraph::print() const {
    for (int i = 0; i < vertices; i++) {
        std::cout << i << ": ";

        for (int j = 0; j < adjList[i].getSize(); j++) {
            Edge edge = adjList[i].get(j);

            std::cout << "("
                      << edge.getEnd()
                      << ", waga: "
                      << edge.getWeight()
                      << ") ";
        }

        std::cout << std::endl;
    }
}
int ListGraph::getVertices() const {
    return vertices;
}

LinkedList<Edge>& ListGraph::getAdjList(int vertex) const {
    return adjList[vertex];
}