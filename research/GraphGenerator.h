#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class GraphGenerator {
public:
    static void generateMatrixGraph(
        MatrixGraph& graph,
        int vertices,
        int density
    );

    static void generateListGraph(
        ListGraph& graph,
        int vertices,
        int density
    );
};

#endif