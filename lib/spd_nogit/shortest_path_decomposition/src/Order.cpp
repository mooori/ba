#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "../include/Path.hpp"
#include "../include/Help.hpp"
#include "../include/Order.hpp"

Order::Order() {
}

Order::Order(Order const &o) {
    path = o.path;
    this->positions = o.positions;
}

Order& Order::operator=(Order const &o) {
    this->path = o.path;
    this->positions = o.positions;
    return *this;
}

Order::~Order() {
}

void Order::addFront(Vertex_Descr v) {
    path.insert(path.begin(), v);
    for (std::map<Vertex_Descr, int>::iterator it_pos = positions.begin(); it_pos != positions.end(); it_pos++)
        it_pos->second = it_pos->second+1;
    positions[v] = 0;
}

void Order::addEnd(Vertex_Descr v) {
    path.push_back(v);
    positions[v] = path.size()-1;
}

void Order::concatenate(Path &p) {
    Vertex_Vec vertices = p.getVertices();
    for (Vertex_Vec::const_iterator it = vertices.begin(); it != vertices.end(); it++) {
        addEnd(*it);
    }
}

//dont change positions map in the next two functions since its not needed
void Order::removeFirst() {
    if (!path.empty())
        path.erase(path.begin());
}

void Order::removeLast() {
    if (!path.empty())
        path.pop_back();
}

int Order::getPosition(Vertex_Descr v) {
    return positions[v];
}

bool Order::lessThan(Vertex_Descr u, Vertex_Descr v) {
    int pos_u = positions[u];
    int pos_v = positions[v];
    
    if (pos_u == -1 || pos_v == -1) {
        Help::output("VERTEX IS NOT IN ORDER");
        std::cin.get();
        exit(-1);
    }
    
    if (pos_u < pos_v)
        return true;
    else
        return false;
}
