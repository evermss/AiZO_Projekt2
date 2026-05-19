#include "MatrixGraph.h"
#include <iostream>

// Konstruktor
MatrixGraph::MatrixGraph(int vertices) : vertices(vertices) {
    matrix = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        matrix[i] = new int[vertices];

        for (int j = 0; j < vertices; j++) {
            matrix[i][j] = 0;
        }
    }
}

// Destruktor
MatrixGraph::~MatrixGraph() {
    for (int i = 0; i < vertices; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

// Dodanie krawedzi
void MatrixGraph::addEdge(int start, int end, int weight) {
    matrix[start][end] = weight;
}

// Wyswietlanie grafu
void MatrixGraph::print() const {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            std::cout << matrix[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int MatrixGraph::getVertices() const {
    return vertices;
}

int MatrixGraph::getEdge(int start, int end) const {
    return matrix[start][end];
}