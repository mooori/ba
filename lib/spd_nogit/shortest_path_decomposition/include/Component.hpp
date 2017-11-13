#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "GraphWrapper.hpp"
#include "MinorModel.hpp"

class Component {
public:
    Component(GraphWrapper* cGraph, MinorModel *mm);
    
    Component();
    
    Component& operator=(Component &c_in);
    
    ~Component();
    
    void smashVertex(Vertex_Descr v);
    
    std::vector<Component*> smashPath(Vertex_Vec &vertices, int choose_arb_mode);
    
    Vertex_Descr growAlongPath(Path* p, bool direction);
    
    bool repairEdge(MinorModel::MMEdge edge);
    
    void print();
    
    void printConnectionEdges();
    
    bool hasConnectionToComponent(Vertex_Descr v);
    
    std::pair<Path*, std::pair<Vertex_Descr, Vertex_Descr> > shortestPath(Blob* b1, Blob* b2);
    
    bool test(bool debug);
    
    bool isInVec(Vertex_Vec vec, Vertex_Descr v);
    
    GraphWrapper* graph;
    MinorModel* minor_model;
    std::vector<Edge> connection_edges;
    Vertex_Descr next_arbitrary_vertex;
};