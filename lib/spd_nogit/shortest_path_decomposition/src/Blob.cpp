#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "../include/Help.hpp"
#include "../include/Blob.hpp"


typedef std::map<Blob*, Vertex_Descr> PebbleMap;

Blob::Blob() {
    id = Help::chars[rand() % 15];
};

Blob::Blob(Blob const &b) {
    connections_to_component = b.connections_to_component;
    pebbles = b.pebbles;
    id = b.id;
}

Blob& Blob::operator=(Blob const &b) {
    connections_to_component = b.connections_to_component;
    pebbles = b.pebbles;
    id = b.id;
    return *this;
}

Blob::~Blob() {
}

void Blob::reset() { //sets all vertices on -1, removes connected vertices
    for (PebbleMap::iterator it_pebbles = pebbles.begin(); it_pebbles != pebbles.end(); it_pebbles++) {
        it_pebbles->second = Help::null_vertex;
    }
    connections_to_component.clear();
}

void Blob::setPebblesOnConnection() {
    for (PebbleMap::iterator it_pebbles = pebbles.begin(); it_pebbles != pebbles.end(); it_pebbles++) {
        if (it_pebbles->second == Help::null_vertex)
            it_pebbles->second = connections_to_component[0];
    }
}

Vertex_Vec& Blob::getConnectedVertices() {
    return connections_to_component;
}

void Blob::addConnectedVertex(Vertex_Descr v) {
    if (!hasVertex(v))
        connections_to_component.push_back(v);
}

void Blob::setPebble(Blob* b, Vertex_Descr v) {
    if (hasPebble(b)) {
        Vertex_Descr old = getPebble(b);
        
        pebbles[b] = v;
        
        removeIfNotUsed(old);
    }
    
    else
        pebbles[b] = v;
    
    if (!hasVertex(v) && v != (Vertex_Descr)-1) {
        connections_to_component.push_back(v);
    }
}

void Blob::removePebble(Blob* b) {
    Vertex_Descr v = pebbles[b];
    pebbles.erase(b);
    removeIfNotUsed(v);
}

Vertex_Descr& Blob::getPebble(Blob* b) {
    return pebbles[b];
}

bool Blob::hasVertex(Vertex_Descr v) {
    for (Vertex_Vec::const_iterator it_connections = connections_to_component.begin(); it_connections != connections_to_component.end(); it_connections++) {
        if (*it_connections == v)
            return true;
    }
    return false;
}

bool Blob::hasPebble(Blob* b) {
    for (PebbleMap::const_iterator it_pebbles = pebbles.begin(); it_pebbles != pebbles.end(); it_pebbles++) {
        if (it_pebbles->first == b)
            return true;
    }
    return false;
}

void Blob::removeConnectedVertex(Vertex_Descr v) {
    for (PebbleMap::iterator it_pebbles = pebbles.begin(); it_pebbles != pebbles.end(); it_pebbles++) {
        if (it_pebbles->second == v)
            it_pebbles->second = Help::null_vertex;
    }
    removeIfNotUsed(v);
}

unsigned int Blob::nPebbles() {
    return pebbles.size();
}

bool Blob::hasConnection() {
    if (connections_to_component.size() > 0)
        return true;
    else
        return false;
}

void Blob::print() {
    std::cout << "(" << id << ")" << std::endl;
    
    
    for (std::map<Blob*, Vertex_Descr>::iterator it = pebbles.begin(); it != pebbles.end(); it++) {
        std::cout << "     " << "(" << it->first->id << "): " << *((int*) &it->second) << std::endl;
    }
    
    
    Help::output();
    for (Vertex_Vec::iterator it = connections_to_component.begin(); it != connections_to_component.end(); it++) {
        std::cout << *((int*) &(*it)) << " ";
    }
    Help::output();
    Help::output();
}

const char* Blob::getID() {
    return id;
}

void Blob::removeIfNotUsed(Vertex_Descr u) {
    bool found = false;
    for (PebbleMap::iterator it_pebbles = pebbles.begin(); it_pebbles != pebbles.end(); it_pebbles++) {
        if (it_pebbles->second == u)
            found = true;
    }
    if (!found && u != (Vertex_Descr)-1)
        connections_to_component.erase(std::find(connections_to_component.begin(), connections_to_component.end(), u));
}

