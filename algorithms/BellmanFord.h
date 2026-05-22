#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class BellmanFord {
public:
    static void runMatrix(MatrixGraph& graph, int startVertex);
    static void runList(ListGraph& graph, int startVertex);
};

#endif