#include <iostream>
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"

int main() {
    // Tworzenie grafów w dwóch reprezentacjach
    MatrixGraph graph(4);
    ListGraph listGraph(4);

    // Wczytywanie grafu z pliku
    FileLoader::loadMatrixGraph("../data/graph.txt", graph);
    FileLoader::loadListGraph("../data/graph.txt", listGraph);

    std::cout << "\nMacierz grafu:\n";
    graph.print();

    std::cout << "\nLista grafu:\n";
    listGraph.print();

    return 0;
}