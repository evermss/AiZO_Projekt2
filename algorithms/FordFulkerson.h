#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class FordFulkerson {
public:
    static void runMatrix(MatrixGraph& graph, int source, int sink);
    static void runList(ListGraph& graph, int source, int sink);
};

#endif