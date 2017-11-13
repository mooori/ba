#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"

class Blob {
public:
    typedef std::map<Blob*, Vertex_Descr> PebbleMap;
    
    Blob();
    
    Blob(Blob const &b);
    
    Blob& operator=(Blob const &b);
    
    ~Blob();
    
    void reset();
    
    void setPebblesOnConnection();
    
    Vertex_Vec& getConnectedVertices();
    
    void addConnectedVertex(Vertex_Descr v);
    
    void setPebble(Blob* b, Vertex_Descr v);
    
    void removePebble(Blob* b);
    
    Vertex_Descr& getPebble(Blob* b);
    
    bool hasVertex(Vertex_Descr v);
    
    bool hasPebble(Blob* b);
    
    void removeConnectedVertex(Vertex_Descr v);
    
    unsigned int nPebbles();
    
    bool hasConnection();
    
    void print();
    
    const char* getID();
    
    PebbleMap pebbles;
     
private:
    void removeIfNotUsed(Vertex_Descr u);
    Vertex_Vec connections_to_component;
    const char* id;
};