#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>

#include <queue>
#include <stack>

#include "Common.h"
#include "Path.hpp"

class Order : public Path {
public:
    Order();
    
    Order(Order const &o);
    
    Order& operator=(Order const &o);
    
    ~Order();
    
    void addFront(Vertex_Descr v);
    
    void addEnd(Vertex_Descr v);
    
    void concatenate(Path &p);
    
    void removeFirst();
    
    void removeLast();
    
    int getPosition(Vertex_Descr v);
    
    bool lessThan(Vertex_Descr u, Vertex_Descr v);
    
private:
    std::map<Vertex_Descr, int> positions;
};