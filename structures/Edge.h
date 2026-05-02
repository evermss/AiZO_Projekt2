#ifndef EDGE_H
#define EDGE_H

// Reprezentacja krawędź grafu
class Edge {
private:
    int start;
    int end;
    int weight;  

public:
    Edge();
    Edge(int start, int end, int weight); // konstruktor

    int getStart() const;
    int getEnd() const;
    int getWeight() const;
};

#endif