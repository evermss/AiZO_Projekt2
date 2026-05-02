#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "IGraph.h"

// Reprezentacja grafu jako macierz sasiedztwa
class MatrixGraph : public IGraph {
private:
    int vertices;
    int** matrix;

public:
    explicit MatrixGraph(int vertices);
    ~MatrixGraph();

    void addEdge(int start, int end, int weight) override;
    void print() const override;
};

#endif