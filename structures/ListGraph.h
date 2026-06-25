#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include "IGraph.h"
#include "LinkedList.h"
#include "Edge.h"

// Reprezentacja grafu jako lista sasiedztwa
class ListGraph : public IGraph {
private:
    int vertices;
    LinkedList<Edge>* adjList;

public:
    explicit ListGraph(int vertices);
    ~ListGraph();

    void addEdge(int start, int end, int weight) override;
    void print() const override;

    int getVertices() const;
    LinkedList<Edge>& getAdjList(int vertex) const;

    template <typename EdgeConsumer>
    void forEachOutgoingEdge(int vertex, EdgeConsumer consumer) const {
        if (vertex < 0 || vertex >= vertices) {
            return;
        }

        const LinkedList<Edge>& list = adjList[vertex];

        for (int i = 0; i < list.getSize(); i++) {
            const Edge& edge = list.get(i);
            consumer(edge.getEnd(), edge.getWeight());
        }
    }

    template <typename EdgeConsumer>
    void forEachIncidentEdge(int vertex, EdgeConsumer consumer) const {
        if (vertex < 0 || vertex >= vertices) {
            return;
        }

        for (int listIndex = 0; listIndex < vertices; listIndex++) {
            const LinkedList<Edge>& list = adjList[listIndex];

            for (int edgeIndex = 0;
                 edgeIndex < list.getSize();
                 edgeIndex++) {

                const Edge& edge = list.get(edgeIndex);

                if (edge.getStart() == vertex) {
                    consumer(edge.getEnd(), edge.getWeight());
                } else if (edge.getEnd() == vertex) {
                    consumer(edge.getStart(), edge.getWeight());
                }
                 }
        }
    }
    
    template <typename EdgeConsumer>
    void forEachEdge(EdgeConsumer consumer) const {
        for (int vertex = 0; vertex < vertices; vertex++) {
            const LinkedList<Edge>& list = adjList[vertex];

            for (int edgeIndex = 0;
                 edgeIndex < list.getSize();
                 edgeIndex++) {

                const Edge& edge = list.get(edgeIndex);

                consumer(
                    edge.getStart(),
                    edge.getEnd(),
                    edge.getWeight()
                );
                 }
        }
    }
};

#endif