#include "BellmanFord.h"

void BellmanFord::runMatrix(
    const MatrixGraph& graph,
    int startVertex,
    int endVertex,
    bool printResult
) {
    run(graph, startVertex, endVertex, printResult);
}

void BellmanFord::runList(
    const ListGraph& graph,
    int startVertex,
    int endVertex,
    bool printResult
) {
    run(graph, startVertex, endVertex, printResult);
}