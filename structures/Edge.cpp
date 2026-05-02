#include "Edge.h"

// Konstruktor domyślny
Edge::Edge() : start(0), end(0), weight(0) {}

// Konstruktor z parametrami
Edge::Edge(int start, int end, int weight)
    : start(start), end(end), weight(weight) {}

// Gettery
int Edge::getStart() const {
    return start;
}

int Edge::getEnd() const {
    return end;
}

int Edge::getWeight() const {
    return weight;
}