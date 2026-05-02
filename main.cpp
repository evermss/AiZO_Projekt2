#include <iostream>
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"

// Wyswietlenie podstawowej instrukcji programu
void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Tryby programu zostana dodane w kolejnym etapie.\n\n";

    std::cout << "Aktualny test:\n";
    std::cout << " - wczytanie grafu z pliku\n";
    std::cout << " - wyswietlenie grafu w macierzy\n";
    std::cout << " - wyswietlenie grafu w liscie\n";
}

int main() {
    showHelp();

    // Tworzenie grafow w dwoch reprezentacjach
    MatrixGraph matrixGraph(4);
    ListGraph listGraph(4);

    // Wczytywanie grafu z pliku testowego
    FileLoader::loadMatrixGraph("../data/graph.txt", matrixGraph);
    FileLoader::loadListGraph("../data/graph.txt", listGraph);

    std::cout << "\nMacierz grafu:\n";
    matrixGraph.print();

    std::cout << "\nLista grafu:\n";
    listGraph.print();

    return 0;
}