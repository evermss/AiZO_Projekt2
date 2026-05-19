#ifndef PRIM_H
#define PRIM_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class Prim {
public:
    static void runMatrix(MatrixGraph& graph);
    static void runList(ListGraph& graph);
};

#endif