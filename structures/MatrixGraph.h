#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "Edge.h"

// Reprezentacja grafu jako macierz
class MatrixGraph {
private:
    int vertices;     // liczba wierzchołków
    int** matrix;     // macierz sąsiedztwa

public:
    MatrixGraph(int vertices);
    ~MatrixGraph();

    void addEdge(int start, int end, int weight);

    void print() const;
};

#endif