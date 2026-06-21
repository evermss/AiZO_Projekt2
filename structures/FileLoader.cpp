#include "FileLoader.h"
#include <fstream>
#include <iostream>

// Wczytuje graf z pliku do reprezentacji macierzowej
void FileLoader::loadMatrixGraph(const std::string& filename, MatrixGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku." << std::endl;
        return;
    }

    int vertices;
    int edges;

    // Odczyt liczby krawedzi i wierzcholkow
    if (!(file >> vertices >> edges)) {
        std::cout << "Blad: niepoprawna pierwsza linia pliku." << std::endl;
        return;
    }

    graph.reserveEdges(edges);

    int loadedEdges = 0;
    int start;
    int end;
    int weight;

    while (file >> start >> end >> weight) {
        // Sprawdzenie poprawnosci danych krawedzi
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

    // Sprawdzenie zgodnosci liczby krawedzi
    if (loadedEdges != edges) {
        std::cout << "Blad: liczba wczytanych krawedzi nie zgadza sie z plikiem." << std::endl;
        std::cout << "Oczekiwano: " << edges << ", wczytano: " << loadedEdges << std::endl;
    }

    file.close();
}

// Wczytuje graf z pliku do reprezentacji listowej
void FileLoader::loadListGraph(const std::string& filename, ListGraph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku." << std::endl;
        return;
    }

    int vertices;
    int edges;

    // Odczyt liczby wierzcholkow i krawedzi
    if (!(file >> vertices >> edges)) {
        std::cout << "Blad: niepoprawna pierwsza linia pliku." << std::endl;
        return;
    }

    int loadedEdges = 0;
    int start;
    int end;
    int weight;

    while (file >> start >> end >> weight) {
        // Sprawdzenie poprawnosci danych krawedzi
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

    // Sprawdzenie zgodnosci liczby krawedzi
    if (loadedEdges != edges) {
        std::cout << "Blad: liczba wczytanych krawedzi nie zgadza sie z plikiem." << std::endl;
        std::cout << "Oczekiwano: " << edges << ", wczytano: " << loadedEdges << std::endl;
    }

    file.close();
}