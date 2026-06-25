#include "Dijkstra.h"

void Dijkstra::runMatrix(
    const MatrixGraph& graph,
    int startVertex,
    int endVertex,
    bool printResult
) {
    run(graph, startVertex, endVertex, printResult);
}

void Dijkstra::runList(
    const ListGraph& graph,
    int startVertex,
    int endVertex,
    bool printResult
) {
    run(graph, startVertex, endVertex, printResult);
}