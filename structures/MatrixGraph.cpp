#include "MatrixGraph.h"
#include <iostream>

void MatrixGraph::allocateIncidenceMatrix(int capacity) {
    edgeCapacity = capacity;

    if (edgeCapacity < 1) {
        edgeCapacity = 1;
    }

    incidenceMatrix = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        incidenceMatrix[i] = new int[edgeCapacity];

        for (int j = 0; j < edgeCapacity; j++) {
            incidenceMatrix[i][j] = 0;
        }
    }

    edgeStarts = new int[edgeCapacity];
    edgeEnds = new int[edgeCapacity];
    edgeWeights = new int[edgeCapacity];

    for (int i = 0; i < edgeCapacity; i++) {
        edgeStarts[i] = -1;
        edgeEnds[i] = -1;
        edgeWeights[i] = 0;
    }
}

void MatrixGraph::resizeIncidenceMatrix(int newCapacity) {
    if (newCapacity <= edgeCapacity) {
        return;
    }

    int** newMatrix = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        newMatrix[i] = new int[newCapacity];

        for (int j = 0; j < newCapacity; j++) {
            if (j < edgeCount) {
                newMatrix[i][j] = incidenceMatrix[i][j];
            } else {
                newMatrix[i][j] = 0;
            }
        }
    }

    int* newStarts = new int[newCapacity];
    int* newEnds = new int[newCapacity];
    int* newWeights = new int[newCapacity];

    for (int i = 0; i < newCapacity; i++) {
        if (i < edgeCount) {
            newStarts[i] = edgeStarts[i];
            newEnds[i] = edgeEnds[i];
            newWeights[i] = edgeWeights[i];
        } else {
            newStarts[i] = -1;
            newEnds[i] = -1;
            newWeights[i] = 0;
        }
    }

    for (int i = 0; i < vertices; i++) {
        delete[] incidenceMatrix[i];
    }

    delete[] incidenceMatrix;
    delete[] edgeStarts;
    delete[] edgeEnds;
    delete[] edgeWeights;

    incidenceMatrix = newMatrix;
    edgeStarts = newStarts;
    edgeEnds = newEnds;
    edgeWeights = newWeights;
    edgeCapacity = newCapacity;
}

MatrixGraph::MatrixGraph(int vertices)
    : vertices(vertices),
      edgeCount(0),
      edgeCapacity(0),
      incidenceMatrix(nullptr),
      adjacencyMatrix(nullptr),
      edgeStarts(nullptr),
      edgeEnds(nullptr),
      edgeWeights(nullptr) {

    if (this->vertices < 1) {
        this->vertices = 1;
    }

    allocateIncidenceMatrix(this->vertices);

    adjacencyMatrix = new int*[this->vertices];

    for (int i = 0; i < this->vertices; i++) {
        adjacencyMatrix[i] = new int[this->vertices];

        for (int j = 0; j < this->vertices; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
}

MatrixGraph::MatrixGraph(int vertices, int edgeCapacity)
    : MatrixGraph(vertices) {
    reserveEdges(edgeCapacity);
}

MatrixGraph::~MatrixGraph() {
    for (int i = 0; i < vertices; i++) {
        delete[] incidenceMatrix[i];
        delete[] adjacencyMatrix[i];
    }

    delete[] incidenceMatrix;
    delete[] adjacencyMatrix;
    delete[] edgeStarts;
    delete[] edgeEnds;
    delete[] edgeWeights;
}

MatrixGraph::MatrixGraph(MatrixGraph&& other) noexcept
    : vertices(other.vertices),
      edgeCount(other.edgeCount),
      edgeCapacity(other.edgeCapacity),
      incidenceMatrix(other.incidenceMatrix),
      adjacencyMatrix(other.adjacencyMatrix),
      edgeStarts(other.edgeStarts),
      edgeEnds(other.edgeEnds),
      edgeWeights(other.edgeWeights) {

    other.vertices = 0;
    other.edgeCount = 0;
    other.edgeCapacity = 0;
    other.incidenceMatrix = nullptr;
    other.adjacencyMatrix = nullptr;
    other.edgeStarts = nullptr;
    other.edgeEnds = nullptr;
    other.edgeWeights = nullptr;
}

MatrixGraph& MatrixGraph::operator=(MatrixGraph&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < vertices; i++) {
            delete[] incidenceMatrix[i];
            delete[] adjacencyMatrix[i];
        }

        delete[] incidenceMatrix;
        delete[] adjacencyMatrix;
        delete[] edgeStarts;
        delete[] edgeEnds;
        delete[] edgeWeights;

        vertices = other.vertices;
        edgeCount = other.edgeCount;
        edgeCapacity = other.edgeCapacity;
        incidenceMatrix = other.incidenceMatrix;
        adjacencyMatrix = other.adjacencyMatrix;
        edgeStarts = other.edgeStarts;
        edgeEnds = other.edgeEnds;
        edgeWeights = other.edgeWeights;

        other.vertices = 0;
        other.edgeCount = 0;
        other.edgeCapacity = 0;
        other.incidenceMatrix = nullptr;
        other.adjacencyMatrix = nullptr;
        other.edgeStarts = nullptr;
        other.edgeEnds = nullptr;
        other.edgeWeights = nullptr;
    }

    return *this;
}

void MatrixGraph::reserveEdges(int capacity) {
    if (capacity > edgeCapacity) {
        resizeIncidenceMatrix(capacity);
    }
}

void MatrixGraph::addEdge(int start, int end, int weight) {
    if (start < 0 || start >= vertices || end < 0 || end >= vertices || weight <= 0) {
        return;
    }

    if (edgeCount >= edgeCapacity) {
        resizeIncidenceMatrix(edgeCapacity * 2);
    }

    for (int i = 0; i < vertices; i++) {
        incidenceMatrix[i][edgeCount] = 0;
    }

    incidenceMatrix[start][edgeCount] = weight;
    incidenceMatrix[end][edgeCount] = -weight;

    edgeStarts[edgeCount] = start;
    edgeEnds[edgeCount] = end;
    edgeWeights[edgeCount] = weight;

    adjacencyMatrix[start][end] = weight;

    edgeCount++;
}

void MatrixGraph::print() const {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < edgeCount; j++) {
            std::cout << incidenceMatrix[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int MatrixGraph::getVertices() const {
    return vertices;
}

int MatrixGraph::getEdgesCount() const {
    return edgeCount;
}

int MatrixGraph::getEdge(int start, int end) const {
    if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
        return 0;
    }

    return adjacencyMatrix[start][end];
}