#include <iostream>
#include "DynamicArray.h"
#include "Edge.h"
#include "LinkedList.h"
#include "MatrixGraph.h"
#include "ListGraph.h"

int main() {
    // Test dynamicznej tablicy
    DynamicArray<Edge> edges;

    edges.add(Edge(0, 1, 10));
    edges.add(Edge(1, 2, 5));

    for (int i = 0; i < edges.getSize(); i++) {
        Edge edge = edges.get(i);

        std::cout << edge.getStart()
                  << " -> "
                  << edge.getEnd()
                  << " waga: "
                  << edge.getWeight()
                  << std::endl;
    }
    // Test listy
    LinkedList<Edge> list;

    list.add(Edge(2, 3, 7));
    list.add(Edge(3, 4, 9));

    for (int i = 0; i < list.getSize(); i++) {
        Edge edge = list.get(i);

        std::cout << edge.getStart()
                  << " -> "
                  << edge.getEnd()
                  << " waga: "
                  << edge.getWeight()
                  << std::endl;
    }

    // Test grafu macierzowego
    MatrixGraph graph(4);

    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 3, 7);

    std::cout << "\nMacierz grafu:\n";
    graph.print();

    // Test grafu listowego
    ListGraph listGraph(4);

    listGraph.addEdge(0, 1, 10);
    listGraph.addEdge(0, 2, 5);
    listGraph.addEdge(1, 3, 7);

    std::cout << "\nLista grafu:\n";
    listGraph.print();
    
    return 0;
}