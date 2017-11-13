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
#include "../include/GraphWrapper.hpp"

namespace b = boost;

GraphWrapper::GraphWrapper(Vertex_Descr const vert_array[], const int sizeV, Edge const edge_array[], const int sizeE) {
    for (int i = 0; i < sizeV; i++) {
        addVertices(vert_array[i]);
    }
    
    for (int i = 0; i < sizeE; i++) {
        Vertex_Descr u = edge_array[i].first;
        Vertex_Descr v = edge_array[i].second;
        addEdges(u, v);
    }
};

GraphWrapper::GraphWrapper() {
};

GraphWrapper::GraphWrapper(GraphWrapper &g_in) {
    Vertex_Map map_index;
    b::associative_property_map<Vertex_Map> prop_map_index(map_index);
    
    b::copy_graph(g_in.g, g, orig_to_copy(prop_map_index));
    
    for (std::map<Vertex_Descr, Vertex_Descr>::iterator it_map = g_in.map_l.begin(); it_map != g_in.map_l.end(); it_map++) { //map_l, map_r anpassen
        map_l[it_map->first] = map_index[it_map->second];
        map_r[map_index[it_map->second]] = it_map->first;
    }
};

GraphWrapper& GraphWrapper::operator=(GraphWrapper &g_in) {
    Vertex_Map map_index;
    b::associative_property_map<Vertex_Map> prop_map_index(map_index);
    
    b::copy_graph(g_in.g, g, orig_to_copy(prop_map_index));
    
    for (std::map<Vertex_Descr, Vertex_Descr>::iterator it_map = g_in.map_l.begin(); it_map != g_in.map_l.end(); it_map++) {
        map_l[it_map->first] = map_index[it_map->second];
        map_r[map_index[it_map->second]] = it_map->first;
    }
    
    return *this;
}

GraphWrapper::~GraphWrapper() {
}

Vertex_Descr GraphWrapper::arbitraryVertex() {
    int size = map_l.size();
    int r = Help::random_int(size);
    Vertex_Map::const_iterator it = map_l.begin();
    
    for (int i = 0; i < r; i++) {
        ++it;
    }
    
    return it->first;
    
}

void GraphWrapper::addVertices(Vertex_Descr v) {
    Vertex_Descr v_new = b::add_vertex(g);
    map_l[v] = v_new;
    map_r[v_new] = v;
}

void GraphWrapper::removeVertex(Vertex_Descr v, bool conv) { //call from outside
    Vertex_Descr v_int;
    if (conv)
        v_int = map_l[v];
    else
        v_int = v;
    
    b::clear_vertex(v_int, g);
    b::remove_vertex(v_int, g);
    
    Vertex_Descr v_map = map_r[v_int];
    map_r.erase(v_int);
    map_l.erase(v_map);
}

void GraphWrapper::removeVertices(Vertex_Vec vertices, bool conv) {
    for (Vertex_Vec::const_iterator it_v = vertices.begin(); it_v != vertices.end(); it_v++) {
        removeVertex(*it_v, conv);
    }
}

Vertex_Descr GraphWrapper::getArbitraryBorderVertex(Vertex_Descr v) { //call from outside
    Vertex_Descr v_int = map_l[v];
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, bool> colors;
    colors[v_int] = true;
    queue.push(v_int);
    
    while (!queue.empty()) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        Vertex_Vec adj = neighbours(curr, false); //call from inside
        bool added_new = false;
        for (Vertex_Vec::iterator it_v = adj.begin(); it_v != adj.end();it_v++) {
            if (colors.find(*it_v) == colors.end()) {
                colors[*it_v] = true;
                queue.push(*it_v);
                added_new = true;
            }
        }
        if (!added_new)
            return map_r[curr];
    }
    return (Vertex_Descr) -1;
}

Path* GraphWrapper::shortestPath(Vertex_Vec sources, Vertex_Vec targets) {
    Vertex_Descr target;
    Path* sp = new Path();
    
    //check if sources and targets overlap
    for (Vertex_Vec::iterator it = sources.begin(); it != sources.end(); it++) {
        for (Vertex_Vec::iterator it_2 = targets.begin(); it_2 != targets.end(); it_2++) {
            if (*it == *it_2) {
                sp->addFront(*it);
                return sp;
            }
        }
    }
    
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, Vertex_Descr> pred;
    std::map<Vertex_Descr, bool> colors; //TODO: change to simple list, no bool needed
    
    for (Vertex_Vec::iterator it = sources.begin(); it != sources.end(); it++) {
        queue.push(map_l[*it]);
        colors[map_l[*it]] = 1;
    }
    
    bool found = false;
    int seed = 0;
    while (!queue.empty() && !found) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        
        Vertex_Vec adj = neighbours(curr, false);
        seed++;
        seed = seed % 7; //random seed, so paths are distributed better
        while (adj.size()) {
            Vertex_Descr it_v;
            if (seed < 3)
                it_v = adj.front();
            else
                it_v = adj.back();
            
            Vertex_Vec::iterator find = std::find(adj.begin(), adj.end(), it_v);
            adj.erase(find);
            
            if (colors.find(it_v) == colors.end()) {
                pred[it_v] = curr;
                if (std::find(targets.begin(), targets.end(), map_r[it_v]) != targets.end()) {
                    found = true;
                    target = it_v;
                    break;
                }
                queue.push(it_v);
                colors[it_v] = 1;
            }
        }
    }
    
    sp->addFront(map_r[target]);
    Vertex_Descr curr = target;
    
    while (pred.find(curr) != pred.end()) {
        curr = pred[curr];
        sp->addFront(map_r[curr]);
    }
    return sp;
}

Vertex_Vec GraphWrapper::neighbours(Vertex_Descr u, bool conv) { //conv = true if call from outside
    Vertex_Vec ret;
    Vertex_Descr u_int;
    if (conv)
        u_int = map_l[u];
    else
        u_int = u;
    std::pair<Adj_Iter, Adj_Iter> adj;
    adj = b::adjacent_vertices(u_int, g);
    for (Adj_Iter it = adj.first; it != adj.second; it++) {
        if (conv)
            ret.push_back(map_r[*it]);
        else
            ret.push_back(*it);
    }
    return ret;
}

Vertex_Vec GraphWrapper::neighbours(Vertex_Vec &vertices, bool conv) {
    Vertex_Vec ret;
    for (Vertex_Vec::iterator it = vertices.begin(); it != vertices.end(); it++) {
        Vertex_Descr u;
        if (conv) {
            u = map_l[*it];
        }
        else {
            u = *it;
        }
        
        Vertex_Vec adj = neighbours(u, false);
        for (Vertex_Vec::const_iterator it = adj.begin(); it != adj.end(); it++) {
            Vertex_Descr v;
            if (conv) {
                v = map_r[*it];
            }
            else {
                v = *it;
            }
            ret.push_back(v);
        }
    }
    return ret;
}

Vertex_Vec GraphWrapper::bfsVertices(Vertex_Descr v, int r, bool conv) { //returns the r neighbourhood as new graph
    Vertex_Vec ret;
    
    Vertex_Descr v_int;
    if (conv)
        v_int = map_l[v];
    else
        v_int = v;
    
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, int> distances;
    distances[v_int] = 0;
    queue.push(v_int);
    ret.push_back(map_r[v_int]);
    
    while (!queue.empty()) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        
        if (distances[curr] < r) {
            Vertex_Vec adj = neighbours(curr, false); //call from inside
            for (Vertex_Vec::iterator it_v = adj.begin(); it_v != adj.end(); it_v++) {
                if (distances.find(*it_v) == distances.end()) {
                    distances[*it_v] = distances[curr] + 1;
                    queue.push(*it_v);
                    ret.push_back(map_r[*it_v]);
                }
            }
        }
    }
    return ret;
}

GraphWrapper* GraphWrapper::bfsGraph(Vertex_Descr v, int r, bool conv) { //returns the r neighbourhood as new graph
    GraphWrapper* ret = new GraphWrapper();
    
    Vertex_Descr v_int;
    if (conv)
        v_int = map_l[v];
    else
        v_int = v;
    
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, int> distances;
    distances[v_int] = 0;
    queue.push(v_int);
    ret->addVertices(map_r[v_int]);
    
    while (!queue.empty()) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        
        if (distances[curr] < r) {
            Vertex_Vec adj = neighbours(curr, false); //call from inside
            for (Vertex_Vec::iterator it_v = adj.begin(); it_v != adj.end(); it_v++) {
                if (distances.find(*it_v) == distances.end()) {
                    distances[*it_v] = distances[curr] + 1;
                    queue.push(*it_v);
                    ret->addVertices(map_r[*it_v]);
                }
                if (!ret->hasEdge(map_r[curr], map_r[*it_v], true))
                    ret->addEdges(map_r[curr], map_r[*it_v]);
            }
        }
    }
    return ret;
}

void GraphWrapper::addEdges(Vertex_Descr u, Vertex_Descr v) {
    Vertex_Descr u_int = map_l[u];
    Vertex_Descr v_int = map_l[v];
    if (!b::edge(u_int, v_int, g).second)
        b::add_edge(u_int, v_int, g);
}

bool GraphWrapper::hasEdge(Vertex_Descr u, Vertex_Descr v, bool conv) {
    Vertex_Descr u_int;
    Vertex_Descr v_int;
    if (conv) {
        u_int = map_l[u];
        v_int = map_l[v];
    }
    else {
        u_int = u;
        v_int = v;
    }
    bool ret = b::edge(u_int, v_int, g).second;
    return ret;
}

Vertex_Vec GraphWrapper::getVertices() {
    Vertex_Vec vertices;
    std::pair<Vertex_Iter, Vertex_Iter> v_it = b::vertices(g);
    
    for (; v_it.first != v_it.second; v_it.first++) {
        vertices.push_back(map_r[*(v_it.first)]);
    }
    
    return vertices;
}

int GraphWrapper::getSize() {
    return b::num_vertices(g);
}

std::vector<Edge> GraphWrapper::getEdges() {
    std::vector<Edge> edges;
    
    std::pair<Edge_Iter, Edge_Iter> e_it = b::edges(g);
    
    for (; e_it.first != e_it.second; e_it.first++) {
        edges.push_back(Edge(map_r[(*(e_it.first)).m_source], map_r[(*(e_it.first)).m_target]));
    }
    
    return edges;
}

void GraphWrapper::print() {
    Help::output("-----------Graph---------");
    std::pair<Vertex_Iter, Vertex_Iter> v_iter = b::vertices(g);
    std::pair<Edge_Iter, Edge_Iter> e_iter = b::edges(g);
    Help::output("vertices: ");
    for (Vertex_Iter v_it = v_iter.first; v_it != v_iter.second; v_it++) {
        std::cout << *((int*) &map_r[(*v_it)]) << " ";
    }
    Help::output();
    Help::output();
    Help::output("edges: ");
    for (Edge_Iter e_it = e_iter.first; e_it != e_iter.second; e_it++) {
        std::cout << *((int*) &map_r[(*e_it).m_source]) << "-" << *((int*) &map_r[(*e_it).m_target]) << " ";
    }
    
    Help::output();
}

Graph GraphWrapper::getGraph() {
    return g;
}

    