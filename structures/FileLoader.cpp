#include "FileLoader.h"
#include <fstream>
#include <iostream>

// Wczytywanie grafu do macierzy
void FileLoader::loadMatrixGraph(const std::string& filename, MatrixGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Nie mozna otworzyc pliku!" << std::endl;
        return;
    }

    int edges, vertices;
    file >> edges >> vertices;

    int start, end, weight;

    while (file >> start >> end >> weight) {
        graph.addEdge(start, end, weight);
    }

    file.close();
}

// Wczytywanie grafu do listy
void FileLoader::loadListGraph(const std::string& filename, ListGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Nie mozna otworzyc pliku!" << std::endl;
        return;
    }

    int edges, vertices;
    file >> edges >> vertices;

    int start, end, weight;

    while (file >> start >> end >> weight) {
        graph.addEdge(start, end, weight);
    }

    file.close();
}