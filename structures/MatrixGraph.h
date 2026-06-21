#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "IGraph.h"

// Reprezentacja grafu jako macierz incydencji.
// Dodatkowa macierz sasiedztwa jest uzywana pomocniczo,
// zeby algorytmy mogly szybko pobierac wage krawedzi getEdge(u, v).
class MatrixGraph : public IGraph {
private:
    int vertices;
    int edgeCount;
    int edgeCapacity;

    int** incidenceMatrix;
    int** adjacencyMatrix;

    int* edgeStarts;
    int* edgeEnds;
    int* edgeWeights;

    void allocateIncidenceMatrix(int capacity);
    void resizeIncidenceMatrix(int newCapacity);

public:
    explicit MatrixGraph(int vertices);
    MatrixGraph(int vertices, int edgeCapacity);

    MatrixGraph(const MatrixGraph& other) = delete;
    MatrixGraph& operator=(const MatrixGraph& other) = delete;

    MatrixGraph(MatrixGraph&& other) noexcept;
    MatrixGraph& operator=(MatrixGraph&& other) noexcept;

    ~MatrixGraph();

    void reserveEdges(int capacity);
    void addEdge(int start, int end, int weight) override;
    void print() const override;

    int getVertices() const;
    int getEdgesCount() const;
    int getEdge(int from, int to) const;
};

#endif