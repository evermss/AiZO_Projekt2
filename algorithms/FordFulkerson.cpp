#include "FordFulkerson.h"

void FordFulkerson::runMatrix(
    const MatrixGraph& graph,
    int source,
    int sink,
    bool printResult
) {
    run(graph, source, sink, printResult);
}

void FordFulkerson::runList(
    const ListGraph& graph,
    int source,
    int sink,
    bool printResult
) {
    run(graph, source, sink, printResult);
}