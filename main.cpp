#include <iostream>
#include "Parameters.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"
#include <string>
#include "Prim.h"
#include "Kruskal.h"
#include "Dijkstra.h"
#include "BellmanFord.h"

// Wyswietlenie podstawowej instrukcji programu
void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Tryby programu:\n";
    std::cout << " - help       -> pomoc\n";
    std::cout << " - singleFile -> pojedynczy test\n";
    std::cout << " - benchmark  -> badania\n";
}

// Tryb pojedynczego testu
void runSingleTest() {
    MatrixGraph matrixGraph(4);
    ListGraph listGraph(4);

    FileLoader::loadMatrixGraph("data/graph.txt", matrixGraph);
    FileLoader::loadListGraph("data/graph.txt", listGraph);

    std::cout << "\nMacierz grafu:\n";
    matrixGraph.print();

    std::cout << "\nLista grafu:\n";
    listGraph.print();

    std::cout << "\nPrim - macierz:\n";
    Prim::runMatrix(matrixGraph);

    std::cout << "\nPrim - lista:\n";
    Prim::runList(listGraph);

    std::cout << "\nKruskal - macierz:\n";
    Kruskal::runMatrix(matrixGraph);

    std::cout << "\nKruskal - lista:\n";
    Kruskal::runList(listGraph);

    std::cout << "\nDijkstra - macierz:\n";
    Dijkstra::runMatrix(matrixGraph, 0);

    std::cout << "\nDijkstra - lista:\n";
    Dijkstra::runList(listGraph, 0);

    std::cout << "\nBellman-Ford - macierz:\n";
    BellmanFord::runMatrix(matrixGraph, 0);

    std::cout << "\nBellman-Ford - lista:\n";
    BellmanFord::runList(listGraph, 0);
}

// Tryb badan
void runBenchmark() {
    std::cout << "Tryb badan zostanie dodany w kolejnym etapie.\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        showHelp();
        runSingleTest();
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "help" || mode == "--help") {
        showHelp();
        return 0;
    }

    if (mode == "singleFile" || mode == "--singleFile") {
        runSingleTest();
        return 0;
    }

    if (mode == "benchmark" || mode == "--benchmark") {
        runBenchmark();
        return 0;
    }

    std::cout << "Nieznany tryb programu.\n";
    showHelp();

    return 0;
}