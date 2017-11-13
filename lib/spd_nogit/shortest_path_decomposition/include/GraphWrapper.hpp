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

class GraphWrapper
{
public:
    GraphWrapper(Vertex_Descr const vert_array[], const int sizeV, Edge const edge_array[], const int sizeE);
    
    GraphWrapper();
    
    GraphWrapper(GraphWrapper &g_in);
    
    GraphWrapper& operator=(GraphWrapper &g_in);
    
    ~GraphWrapper();
    
    Vertex_Descr arbitraryVertex();
    
    void addVertices(Vertex_Descr v);
    
    void removeVertex(Vertex_Descr v, bool conv);
    
    void removeVertices(Vertex_Vec vertices, bool conv);
    
    Vertex_Descr getArbitraryBorderVertex(Vertex_Descr v);
    
    Path* shortestPath(Vertex_Vec sources, Vertex_Vec targets);
    
    Vertex_Vec neighbours(Vertex_Descr u, bool conv);
    
    Vertex_Vec neighbours(Vertex_Vec &vertices, bool conv);
    
    Vertex_Vec bfsVertices(Vertex_Descr v, int r, bool conv);
    
    GraphWrapper* bfsGraph(Vertex_Descr v, int r, bool conv);
    
    void addEdges(Vertex_Descr u, Vertex_Descr v);
    
    bool hasEdge(Vertex_Descr u, Vertex_Descr v, bool conv);
    
    Vertex_Vec getVertices();
    
    int getSize();
    
    std::vector<Edge> getEdges();
    
    void print();
    
    Graph getGraph();
    
private:
    Graph g;
    Vertex_Map map_l, map_r;
};