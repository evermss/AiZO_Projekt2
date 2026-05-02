#ifndef IGRAPH_H
#define IGRAPH_H

// Podstawowy interfejs grafu
class IGraph {
public:
    virtual ~IGraph() = default;

    virtual void addEdge(int start, int end, int weight) = 0;
    virtual void print() const = 0;
};

#endif