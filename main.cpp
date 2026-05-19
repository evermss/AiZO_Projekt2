#include <iostream>
#include "Parameters.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"

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
}

int main(int argc, char* argv[]) {
    // Bez argumentow uruchamiamy prosty test
    if (argc == 1) {
        showHelp();
        runSingleTest();
        return 0;
    }

    // Odczyt parametrow z biblioteki prowadzacego
    Parameters::readParameters(argc, argv);

    if (Parameters::runMode == Parameters::RunModes::help) {
        Parameters::help();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        runSingleTest();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::benchmark) {
        std::cout << "Tryb badan zostanie dodany w kolejnym etapie.\n";
        return 0;
    }

    std::cout << "Nieznany tryb programu.\n";
    showHelp();

    return 0;
}