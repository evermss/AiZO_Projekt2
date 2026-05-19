#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "MatrixGraph.h"
#include "ListGraph.h"

class Kruskal {
public:
    static void runMatrix(MatrixGraph& graph);
    static void runList(ListGraph& graph);
};

#endif