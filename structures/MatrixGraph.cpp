#include "MatrixGraph.h"
#include <iostream>

// Konstruktor
MatrixGraph::MatrixGraph(int vertices) : vertices(vertices) {
    matrix = new int*[vertices];

    for (int i = 0; i < vertices; i++) {
        matrix[i] = new int[vertices];

        for (int j = 0; j < vertices; j++) {
            matrix[i][j] = 0; // brak krawędzi
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

// Dodanie krawędzi
void MatrixGraph::addEdge(int start, int end, int weight) {
    matrix[start][end] = weight;
}

// Wyświetlanie grafu
void MatrixGraph::print() const {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}