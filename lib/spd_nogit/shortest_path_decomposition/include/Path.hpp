#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"

class Path {
public:
    Path();
    
    Path(Path const &p);
    
    Path& operator=(Path const &p);
    
    ~Path();
    
    void addFront(Vertex_Descr v);
    
    void addEnd(Vertex_Descr v);
    
    void concatenate(Path &p);
    
    Vertex_Descr first();
    
    Vertex_Descr last();
    
    Vertex_Vec getVertices();
    
    void removeFirst();
    
    void removeLast();
    
    bool isEmpty();
    
    int getSize();
    
    void print();
    
protected:
    Vertex_Vec path;
};