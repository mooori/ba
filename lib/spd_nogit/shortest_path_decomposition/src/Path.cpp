#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "../include/Help.hpp"
#include "../include/Path.hpp"


Path::Path() {
}

Path::Path(Path const &p) {
    path = p.path;
}

Path& Path::operator=(Path const &p) {
    this->path = p.path;
    return *this;
}

Path::~Path() {
}

void Path::addFront(Vertex_Descr v) {
    path.insert(path.begin(), v);
}

void Path::addEnd(Vertex_Descr v) {
    path.push_back(v);
}

void Path::concatenate(Path &p) {
    Vertex_Vec vertices = p.getVertices();
    for (Vertex_Vec::const_iterator it = vertices.begin(); it != vertices.end(); it++) {
        addEnd(*it);
    }
}

Vertex_Descr Path::first() {
    return path.front();
}

Vertex_Descr Path::last() {
    return path.back();
}

Vertex_Vec Path::getVertices() {
    return path;
}

//dont change positions map in the next two functions since its not needed
void Path::removeFirst() {
    if (!path.empty())
        path.erase(path.begin());
}

void Path::removeLast() {
    if (!path.empty())
        path.pop_back();
}

bool Path::isEmpty() {
    return path.empty();
}

int Path::getSize() {
    return path.size();
}

void Path::print() {
    for (unsigned int i = 0; i < path.size(); i++)
        if (i < path.size() - 1)
            std::cout << *((int*) &path[i]) << " - ";
        else
            std::cout << *((int*) &path[i]);
        Help::output();
}

