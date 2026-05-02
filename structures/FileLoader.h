#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include "MatrixGraph.h"
#include "ListGraph.h"

// Klasa do wczytywania grafu z pliku
class FileLoader {
public:
    static void loadMatrixGraph(const std::string& filename, MatrixGraph& graph);
    static void loadListGraph(const std::string& filename, ListGraph& graph);
};

#endif