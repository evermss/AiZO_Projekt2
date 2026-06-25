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

    template <typename EdgeConsumer>
    void forEachOutgoingEdge(int vertex, EdgeConsumer consumer) const {
        if (vertex < 0 || vertex >= vertices) {
            return;
        }

        for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
            int startValue = incidenceMatrix[vertex][edgeIndex];

            if (startValue <= 0) {
                continue;
            }

            int endVertex = -1;

            for (int row = 0; row < vertices; row++) {
                if (incidenceMatrix[row][edgeIndex] < 0) {
                    endVertex = row;
                    break;
                }
            }

            if (endVertex != -1) {
                consumer(endVertex, startValue);
            }
        }
    }

    template <typename EdgeConsumer>
    void forEachIncidentEdge(int vertex, EdgeConsumer consumer) const {
        if (vertex < 0 || vertex >= vertices) {
            return;
        }

        for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
            int value = incidenceMatrix[vertex][edgeIndex];

            if (value == 0) {
                continue;
            }

            int neighbour = -1;

            for (int row = 0; row < vertices; row++) {
                if (row != vertex &&
                    incidenceMatrix[row][edgeIndex] != 0) {

                    neighbour = row;
                    break;
                    }
            }

            if (neighbour != -1) {
                int weight = value < 0 ? -value : value;
                consumer(neighbour, weight);
            }
        }
    }

    template <typename EdgeConsumer>
    void forEachEdge(EdgeConsumer consumer) const {
        for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
            int startVertex = -1;
            int endVertex = -1;
            int weight = 0;

            for (int row = 0; row < vertices; row++) {
                int value = incidenceMatrix[row][edgeIndex];

                if (value > 0) {
                    startVertex = row;
                    weight = value;
                } else if (value < 0) {
                    endVertex = row;
                }
            }

            if (startVertex != -1 && endVertex != -1) {
                consumer(startVertex, endVertex, weight);
            }
        }
    }
};

#endif