#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include "IGraph.h"
#include "LinkedList.h"
#include "Edge.h"

// Reprezentacja grafu jako lista sasiedztwa
class ListGraph : public IGraph {
private:
    int vertices;
    LinkedList<Edge>* adjList;

public:
    explicit ListGraph(int vertices);
    ~ListGraph();

    void addEdge(int start, int end, int weight) override;
    void print() const override;

    int getVertices() const;
    LinkedList<Edge>& getAdjList(int vertex) const;
};

#endif