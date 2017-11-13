#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "../include/Help.hpp"
#include "../include/GraphWrapper.hpp"
#include "../include/MinorModel.hpp"
#include "../include/Component.hpp"


Component::Component(GraphWrapper* cGraph, MinorModel *mm) {
    graph = cGraph;
    minor_model = mm;
};

Component::Component() {
};

Component& Component::operator=(Component &c_in) {
    graph = new GraphWrapper(*c_in.graph);
    minor_model = new MinorModel(*c_in.minor_model);
    connection_edges = c_in.connection_edges;
    next_arbitrary_vertex = c_in.next_arbitrary_vertex;
    return *this;
}

Component::~Component() {
    delete graph;
    delete minor_model;
};

void Component::smashVertex(Vertex_Descr v) { //also set next_arbitrary_vertex
    //remove connection_edges with v
    for (std::vector<Edge>::iterator it_e = connection_edges.begin(); it_e != connection_edges.end();) {
        if (it_e->first == v) 
            it_e = connection_edges.erase(it_e);
        else 
            it_e++;
    } 
    //add neighbours in graph to connection_edges
    Vertex_Vec adj_temp = graph->neighbours(v, true);
    
    for (Vertex_Vec::const_iterator it_v = adj_temp.begin(); it_v != adj_temp.end(); it_v++) {
        connection_edges.push_back(Edge(*it_v, v));
    }
    
    //remove v from underlying graph
    graph->removeVertex(v, true);
}

std::vector<Component*> Component::smashPath(Vertex_Vec &vertices, int choose_arb_mode) { //removes vertices from graph and returns subgraphs, calls from outside!!!
    Vertex_Vec neighbours;
    std::vector<Edge> connection_edges_temp; //connections from vertices parameter
    
    //prepass, get neighbours and delete
    for (Vertex_Vec::const_iterator it_v = vertices.begin(); it_v != vertices.end(); it_v++) {
        Vertex_Vec adj_temp = graph->neighbours(*it_v, true);
        
        for (Vertex_Vec::const_iterator it_a = adj_temp.begin(); it_a != adj_temp.end(); it_a++) {
            if (std::find(vertices.begin(), vertices.end(), *it_a) == vertices.end()) {
                connection_edges_temp.push_back(Edge(*it_a, *it_v));
                
                if (std::find(neighbours.begin(), neighbours.end(), *it_a) == neighbours.end()) {
                    neighbours.push_back(*it_a);
                }
            }
        }
    }
    
    std::vector<Component*> subcomponents;
    std::map<Vertex_Descr, bool> colors;
    
    this->graph->removeVertices(vertices, true);
    
    //every iteration here creates new component
    while (neighbours.size() > 0) {
        GraphWrapper* g_new = new GraphWrapper();
        MinorModel* m_new = new MinorModel(*minor_model);
        Component* c_new = new Component();
        c_new->graph = g_new;
        c_new->minor_model = m_new;
        
        std::queue<Vertex_Descr> queue;
        Vertex_Descr u = neighbours.front();
        neighbours.erase(neighbours.begin());
        queue.push(u);
        colors[u] = true;
        
        g_new->addVertices(u);
        c_new->next_arbitrary_vertex = u; //set first, but update later
        int max_degree = 0; 
        bool arb_set = false;
        
        //add connection egdes
        for (std::vector<Edge>::iterator it_e = connection_edges_temp.begin(); it_e != connection_edges_temp.end();) {
            if ((*it_e).first == u) {
                c_new->connection_edges.push_back(Edge((*it_e).first, (*it_e).second));
                it_e = connection_edges_temp.erase(it_e);
            }
            else {
                it_e++;
            }
        }
        for (std::vector<Edge>::iterator it_e = connection_edges.begin(); it_e != connection_edges.end();) {
            if ((*it_e).first == u) {
                c_new->connection_edges.push_back(Edge((*it_e).first, (*it_e).second));
                it_e = connection_edges.erase(it_e);
            }
            else {
                it_e++;
            }
        }
        
        //bfs, also set next_arbitrary_vertex
        while (!queue.empty()) {
            Vertex_Descr curr = queue.front();
            queue.pop();
            
            Vertex_Vec adj_temp = this->graph->neighbours(curr, true);
            
            //choose max_degree vertex
            if (choose_arb_mode == 1) {
                int curr_degree;
                if (curr == u) //dont compensate
                    curr_degree = adj_temp.size();
                else 
                    curr_degree = adj_temp.size()+1;
                if (curr_degree > max_degree) {
                    c_new->next_arbitrary_vertex = curr;
                    max_degree = curr_degree; 
                }
            }
            
            bool added_new = false;
            
            for (Vertex_Vec::iterator it = adj_temp.begin(); it != adj_temp.end(); it++) {
                Vertex_Descr it_vert = *it;
                
                if (colors.find(it_vert) == colors.end()) {
                    //delete from neighbours (search sources)
                    Vertex_Vec::iterator f = std::find(neighbours.begin(), neighbours.end(), it_vert);
                    if (f != neighbours.end())
                        neighbours.erase(f);
                    
                    //add vertex to new graph
                    c_new->graph->addVertices(it_vert);
                    added_new = true;
                    
                    //add connection_edges
                    for (std::vector<Edge>::iterator it_e = connection_edges_temp.begin(); it_e != connection_edges_temp.end();) {
                        if ((*it_e).first == it_vert) {
                            c_new->connection_edges.push_back(Edge((*it_e).first, (*it_e).second));
                            it_e = connection_edges_temp.erase(it_e);
                        }
                        else {
                            it_e++;
                        }
                    }
                    for (std::vector<Edge>::iterator it_e = connection_edges.begin(); it_e != connection_edges.end(); it_e++) {
                        if ((*it_e).first == it_vert) {
                            c_new->connection_edges.push_back(Edge(it_vert, (*it_e).second));
                        }
                    }
                    
                    //finish
                    queue.push(it_vert);
                    colors[it_vert] = true;
                }
                
                //add edge
                c_new->graph->addEdges(curr, it_vert);
            }
            
            //choose arbitrary border vertex
            if (choose_arb_mode == 0) {
                if ((!arb_set && !added_new) || queue.empty()) {
                    c_new->next_arbitrary_vertex = curr;
                    arb_set = true;
                }
            }
            
            this->graph->removeVertex(curr, true);
            colors.erase(curr);
        }
        subcomponents.push_back(c_new);
    }
    return subcomponents;
}

Vertex_Descr Component::growAlongPath(Path* p, bool direction) { //returns first connected vertex, -1 otherwise, and shortens the path, 0:forward, 1:backwards
    Vertex_Descr curr = (Vertex_Descr) 0;
    if (!direction) { //grow forward
        while (p->getSize() >= 2) {
            curr = p->first();
            if (hasConnectionToComponent(curr)) {
                return curr;
            }
            else if (p->getSize() > 2) {
                p->removeFirst();
            }
            else
                return (Vertex_Descr)-1;
        }
    }
    else { //grow backwards
        while (p->getSize() >= 2) {
            curr = p->last();
            if (hasConnectionToComponent(curr)) {
                return curr;
            }
            else if (p->getSize() > 2) {
                p->removeLast();
            }
            else //reached end and have no connection
                return (Vertex_Descr)-1;
        }
    }
    return curr;
}

bool Component::repairEdge(MinorModel::MMEdge edge) { //inv: endpoints of p lie in b1 resp. b2, grows in both directions, deletes if one direction fails, returns if successful
    Path* p = edge.first;
    Vertex_Descr start, end;
    Vertex_Descr null = (Vertex_Descr)-1;
    Blob* b1 = edge.second.first;
    Blob* b2 = edge.second.second;
    int direction;
    bool repaired = true;
    
    start = p->first();
    end = p->last();
    
    //which direction
    if (b1->hasVertex(start) && b2->hasVertex(end)) {
        direction = 0; //b1 -> b2
    }
    else if (b1->hasVertex(end) && b2->hasVertex(start)) {
        direction = 1; //b2 -> b1
    }
    else {
        std::cout << "Path ";
        p->print();
        std::cout << "doesn't connect " << b1 << " and " << b2 << std::endl;
        std::cin.get();
        exit(-1);
        return false;
    }
    
    if (!direction) { //b1 -> b2
        Vertex_Descr new_connection_b1 = growAlongPath(p, 0);
        Vertex_Descr new_connection_b2 = growAlongPath(p, 1);
        
        if (new_connection_b1 == null) {
            repaired = false;
            b1->setPebble(b2, (Vertex_Descr)-1); //removes old, if not used
        }
        else {
            b1->setPebble(b2, new_connection_b1);
        }
        
        if (new_connection_b2 == null) {
            repaired = false;
            b2->setPebble(b1, (Vertex_Descr)-1);
        }
        else {
            b2->setPebble(b1, new_connection_b2);
        }
    }
    else {
        Vertex_Descr new_connection_b2 = growAlongPath(p, 0);
        Vertex_Descr new_connection_b1 = growAlongPath(p, 1);
        
        if (new_connection_b2 == null) {
            repaired = false;
            b2->setPebble(b1, (Vertex_Descr)-1);
        }
        else {
            b2->setPebble(b1, new_connection_b2);
        }
        
        if (new_connection_b1 == null) {
            repaired = false;
            b1->setPebble(b2, (Vertex_Descr)-1);
        }
        else {
            b1->setPebble(b2, new_connection_b1);
        }
    }
    
    return repaired;
}

void Component::print() {
    Help::output("**********Component**********");
    graph->print();
    Help::output("connection_edges:");
    for (std::vector<Edge>::iterator e_it = connection_edges.begin(); e_it != connection_edges.end(); e_it++) {
        std::cout << *((int*) &(e_it->first)) << " -> (" << *((int*) &(*e_it).second) << ")" << std::endl;
    }
    Help::output();
    minor_model->print();
    Help::output("*****************************");
    Help::output();
}

void Component::printConnectionEdges() {
    Help::output("connection_edges:");
    for (std::vector<Edge>::iterator e_it = connection_edges.begin(); e_it != connection_edges.end(); e_it++) {
        std::cout << *((int*) &(*e_it).first) << " -> (" << *((int*) &(*e_it).second) << ")" << std::endl;
    }
    Help::output();
}

bool Component::hasConnectionToComponent(Vertex_Descr v) {
    for (std::vector<Edge>::iterator it = connection_edges.begin(); it != connection_edges.end(); it++) {
        if (it->second == v) {
            return true;
        }
    }
    return false;
}

std::pair<Path*, std::pair<Vertex_Descr, Vertex_Descr> > Component::shortestPath(Blob* b1, Blob* b2) {
    Vertex_Vec sources_blob = b1->getConnectedVertices();
    Vertex_Vec targets_blob = b2->getConnectedVertices();
    
    Vertex_Vec sources_comp, targets_comp;
    for (std::vector<Edge>::iterator it = connection_edges.begin(); it != connection_edges.end(); it++) {
        Vertex_Descr c_v = it->first;
        Vertex_Descr b_v = it->second;
        
        if (Component::isInVec(sources_blob, b_v)) {
            if (!isInVec(sources_comp, c_v))
                sources_comp.push_back(c_v);
        }
        else if (isInVec(targets_blob, b_v)) {
            if (!isInVec(targets_comp, c_v))
                targets_comp.push_back(c_v);
        }
    }
    
    Path* sp = graph->shortestPath(sources_comp, targets_comp);
    
    Vertex_Descr start = sp->first();
    Vertex_Descr end = sp->last();
    Vertex_Descr pebble_start, pebble_end;
    
    for (std::vector<Edge>::iterator it = Component::connection_edges.begin(); it != Component::connection_edges.end(); it++) {
        if (it->first == start && b1->hasVertex(it->second)) {
            pebble_start = it->second;
            break;
        }
    }
    
    for (std::vector<Edge>::iterator it = connection_edges.begin(); it != connection_edges.end(); it++) {
        if (it->first == end && b2->hasVertex(it->second)) {
            pebble_end = it->second;
            break;
        }
    }
    
    return std::make_pair(sp, std::make_pair(pebble_start, pebble_end));
}

bool Component::test(bool debug) {
    bool ret = true;
    
    ret = minor_model->test(debug);
    
    if (debug)
        Help::output("Tests of component...");
    
    //connection points of blobs are in connection Edges
    bool test = true;
    for (MinorModel::BlobPVec::iterator it = minor_model->blobs.begin(); it != minor_model->blobs.end(); it++) {
        Vertex_Vec connections = (*it)->getConnectedVertices();
        for (Vertex_Vec::iterator it_2 = connections.begin(); it_2 != connections.end(); it_2++) {
            if (!hasConnectionToComponent(*it_2)) {
                test = false;
                ret = false;
                if (debug)
                    std::cout << "Vertex " << *((int*) &(*it_2)) << " in blob " << (*it)->getID() << " has no connection to component!" << std::endl;
            }
        }
    }
    
    if (debug) {
        Help::output("Test 1: Are vertices in Blobs connected?...");
        if (test)
            Help::output("OK!");
        else
            Help::output("ERROR");
    }
    
    return ret;
}

bool Component::isInVec(Vertex_Vec vec, Vertex_Descr v) {
    if (std::find(vec.begin(), vec.end(), v) != vec.end())
        return true;
    else
        return false;
}
