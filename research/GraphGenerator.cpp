#include "GraphGenerator.h"
#include <cstdlib>
#include <ctime>

int getRandomWeight(int edgesCount) {
    int maxWeight = (edgesCount * 4) / 5;

    if (maxWeight < 1) {
        maxWeight = 1;
    }

    return (rand() % maxWeight) + 1;
}

void GraphGenerator::generateMatrixGraph(
    MatrixGraph& graph,
    int vertices,
    int density
) {
    int maxEdges = vertices * (vertices - 1) / 2;
    int edgesToGenerate = (maxEdges * density) / 100;

    srand(time(nullptr));
    graph.reserveEdges(edgesToGenerate);

    int generatedEdges = 0;

    // Tworzymy drzewo, zeby graf byl spojny
    for (int i = 1; i < vertices; i++) {
        int start = rand() % i;
        int end = i;

        int weight = getRandomWeight(edgesToGenerate);

        graph.addEdge(start, end, weight);
        generatedEdges++;
    }

    //Dodajemy losowe krawedzie do wymaganej gestosci
    while (generatedEdges < edgesToGenerate) {
        int start = rand() % vertices;
        int end = rand() % vertices;

        if (start != end && graph.getEdge(start, end) == 0) {
            int weight = getRandomWeight(edgesToGenerate);

            graph.addEdge(start, end, weight);
            generatedEdges++;
        }
    }
}

void GraphGenerator::generateListGraph(
    ListGraph& graph,
    int vertices,
    int density
) {
    int maxEdges = vertices * (vertices - 1);
    int edgesToGenerate = (maxEdges * density) / 100;

    srand(time(nullptr));

    int generatedEdges = 0;

    // Najpier drzewo, zeby graf byl spojny
    for (int i = 1; i < vertices; i++) {
        int start = rand() % i;
        int end = i;

        int weight = getRandomWeight(edgesToGenerate);

        graph.addEdge(start, end, weight);
        generatedEdges++;
    }

    // Potem losowe krawedzie do wymaganej gestosci
    while (generatedEdges < edgesToGenerate) {
        int start = rand() % vertices;
        int end = rand() % vertices;

        if (start != end) {
            int weight = getRandomWeight(edgesToGenerate);

            graph.addEdge(start, end, weight);
            generatedEdges++;
        }
    }
}