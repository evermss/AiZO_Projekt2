#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class Dijkstra {
public:
    static void runMatrix(MatrixGraph& graph, int startVertex);
    static void runList(ListGraph& graph, int startVertex);
};

#endif