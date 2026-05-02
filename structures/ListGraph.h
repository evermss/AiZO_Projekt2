#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include "LinkedList.h"
#include "Edge.h"

// Reprezentacja grafu jako lista sąsiedztwa
class ListGraph {
private:
    int vertices;              // liczba wierzchołków
    LinkedList<Edge>* adjList; // tablica list sąsiedztwa

public:
    explicit ListGraph(int vertices);
    ~ListGraph();

    void addEdge(int start, int end, int weight);
    void print() const;
};

#endif