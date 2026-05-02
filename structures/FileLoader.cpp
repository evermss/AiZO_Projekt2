#include "FileLoader.h"
#include <fstream>
#include <iostream>

// Wczytywanie grafu do macierzy
void FileLoader::loadMatrixGraph(const std::string& filename, MatrixGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku." << std::endl;
        return;
    }

    int edges;
    int vertices;

    if (!(file >> edges >> vertices)) {
        std::cout << "Blad: niepoprawna pierwsza linia pliku." << std::endl;
        return;
    }

    int loadedEdges = 0;
    int start;
    int end;
    int weight;

    while (file >> start >> end >> weight) {
        if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
            std::cout << "Blad: niepoprawny numer wierzcholka." << std::endl;
            continue;
        }

        if (weight <= 0) {
            std::cout << "Blad: waga musi byc dodatnia." << std::endl;
            continue;
        }

        graph.addEdge(start, end, weight);
        loadedEdges++;
    }

    if (loadedEdges != edges) {
        std::cout << "Blad: liczba wczytanych krawedzi nie zgadza sie z plikiem." << std::endl;
        std::cout << "Oczekiwano: " << edges << ", wczytano: " << loadedEdges << std::endl;
    }

    file.close();
}

// Wczytywanie grafu do listy
void FileLoader::loadListGraph(const std::string& filename, ListGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku." << std::endl;
        return;
    }

    int edges;
    int vertices;

    if (!(file >> edges >> vertices)) {
        std::cout << "Blad: niepoprawna pierwsza linia pliku." << std::endl;
        return;
    }

    int loadedEdges = 0;
    int start;
    int end;
    int weight;

    while (file >> start >> end >> weight) {
        if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
            std::cout << "Blad: niepoprawny numer wierzcholka." << std::endl;
            continue;
        }

        if (weight <= 0) {
            std::cout << "Blad: waga musi byc dodatnia." << std::endl;
            continue;
        }

        graph.addEdge(start, end, weight);
        loadedEdges++;
    }

    if (loadedEdges != edges) {
        std::cout << "Blad: liczba wczytanych krawedzi nie zgadza sie z plikiem." << std::endl;
        std::cout << "Oczekiwano: " << edges << ", wczytano: " << loadedEdges << std::endl;
    }

    file.close();
}