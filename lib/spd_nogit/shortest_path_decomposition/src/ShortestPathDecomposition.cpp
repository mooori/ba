#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "../include/Common.h"
#include "../include/Help.hpp"
#include "../include/Order.hpp"
#include "../include/Component.hpp"
#include "../include/ShortestPathDecomposition.hpp"

ShortestPathDecomposition::ShortestPathDecomposer::ShortestPathDecomposer() {
    test_result = true;
}

ShortestPathDecomposition::ShortestPathDecomposer::~ShortestPathDecomposer() {
    delete g;
}

void ShortestPathDecomposition::ShortestPathDecomposer::init(Component* c_in) {
    c = c_in;
    g = new GraphWrapper(*(c_in->graph)); 
}

void ShortestPathDecomposition::ShortestPathDecomposer::init(Graph &g_in) {
    
    //create component
    c = new Component();
    c->minor_model = new MinorModel();
    c->graph = new GraphWrapper();
    
    std::pair<Vertex_Iter, Vertex_Iter> it_v = b::vertices(g_in);
    std::pair<Edge_Iter, Edge_Iter> it_e = b::edges(g_in);
    
    Vertex_Descr max_degree_v;
    
    unsigned highest_degree = 0;
    for (;it_v.first != it_v.second; it_v.first++) {
        c->graph->addVertices(*(it_v.first));
        unsigned int curr_degree = b::out_degree(*(it_v.first), g_in);
        if (curr_degree > highest_degree) {
            highest_degree = curr_degree;
            max_degree_v = *(it_v.first);
        }
    }
    
    for (;it_e.first != it_e.second; it_e.first++) {
        c->graph->addEdges(b::source(*(it_e.first), g_in), b::target(*(it_e.first), g_in));
    }
    
    c->next_arbitrary_vertex = max_degree_v;
    g = new GraphWrapper(*(c->graph)); 
}

void ShortestPathDecomposition::ShortestPathDecomposer::decompose(Config config) {
    //todo: calculate next_arb vertex
    std::stack<Component*> component_stack;
    unsigned int n_vertices = c->graph->getSize();
    unsigned int max_t = 0;
    unsigned int max_stack_size = 0;
    unsigned int i = 0;
    
    //set arbitrary vertex
    if (config.choose_arb_mode == 0) {
        c->next_arbitrary_vertex = c->graph->getArbitraryBorderVertex(c->next_arbitrary_vertex);
    }
    
    component_stack.push(c);
    
    while (!component_stack.empty()) {
        if (component_stack.size() > max_stack_size)
            max_stack_size = component_stack.size();
        
        Component* curr_component = component_stack.top();
        MinorModel* curr_mm = curr_component->minor_model;
        GraphWrapper* curr_g = curr_component->graph;
        
        component_stack.pop();
        
        if (config.debug) {
            std::cin.get();
            std::cout << ".......... " << i << "th step of Decomposition. " << ".........." << std::endl;
            if (config.debug == 2) {
                curr_g->print();
                curr_component->printConnectionEdges();
                Help::output();
                Help::output("before repair...");
                curr_mm->print();
            }
        }
        
        //+++repair+++ 
        //repair paths
        for (MinorModel::MMEdges::iterator it_paths = curr_mm->paths.begin(); it_paths != curr_mm->paths.end();) {
            if (curr_component->repairEdge(*it_paths)) {
                it_paths++;
            }
            else {
                it_paths = curr_mm->removePath(it_paths->first);
            }
        }
        
        //check for orphaned blobs
        for (MinorModel::BlobPVec::iterator it_blobs = curr_mm->blobs.begin(); it_blobs != curr_mm->blobs.end();) {
            Vertex_Vec old_connections = (*it_blobs)->getConnectedVertices();
            for (Vertex_Vec::iterator it_old_connections = old_connections.begin(); it_old_connections != old_connections.end(); it_old_connections++) {
                if (!curr_component->hasConnectionToComponent(*it_old_connections))
                    (*it_blobs)->removeConnectedVertex(*it_old_connections);
            }
            
            if ((*it_blobs)->hasConnection()) {
                (*it_blobs)->setPebblesOnConnection();
                it_blobs++;
            }
            
            else {
                for (MinorModel::BlobPVec::iterator it_blobs_2 = curr_mm->blobs.begin(); it_blobs_2 != curr_mm->blobs.end(); it_blobs_2++) {
                    if (*it_blobs_2 != *it_blobs)
                        if ((*it_blobs_2)->hasPebble(*it_blobs))
                            (*it_blobs_2)->removePebble(*it_blobs);
                }
                it_blobs = curr_mm->removeBlob(*it_blobs);
            }
        }
        
        //after repair outputs
        if (config.debug) {
            std::cin.get();
            if (config.debug == 2) {
                Help::output("after repair...");
                curr_mm->print();
            }
        }
        
        if (config.test) {
            if (!curr_component->test(config.debug)) {
                test_result = false;
                std::cin.get();
            }
        }
        
        //t calculation
        unsigned int curr_mm_t = curr_mm->getT();
        if (curr_mm_t > max_t) {
            max_t = curr_mm_t;
        }
        
        //hasUnconnected blobs
        if (!curr_mm->isFull()) { 
            MinorModel::BlobPointerPair blobs = curr_mm->nextUnconnectedBlobs();
            std::pair<Path*, std::pair<Vertex_Descr, Vertex_Descr> > sp_return = curr_component->shortestPath(blobs.first, blobs.second);
            
            if (config.debug) {
                std::cout << "minor model has unconnected blobs, calculate shortest path between " << blobs.first->getID() << " and " << blobs.second->getID() << "... " << std::endl;
                sp_return.first->print();
                Help::output();
            }
            
            order.concatenate(*(sp_return.first));
            
            Vertex_Vec smash_verts = sp_return.first->getVertices();
            
            Path* new_path = new Path(*sp_return.first);
            delete sp_return.first;
            new_path->addFront(sp_return.second.first);
            new_path->addEnd(sp_return.second.second);
            curr_mm->addPath(new_path, blobs.first, blobs.second);
            
            std::vector<Component*> new_comps = curr_component->smashPath(smash_verts, config.choose_arb_mode);
            
            for (std::vector<Component*>::iterator it = new_comps.begin(); it != new_comps.end(); it++)
                component_stack.push(*it);
            
            delete curr_component;
        }
        
        //choose arbitrary vertex (border or max degree) in this step
        else {
            Vertex_Descr arb = curr_component->next_arbitrary_vertex;
            
            if (config.choose_arb_mode == 0) {
                curr_component->next_arbitrary_vertex = curr_g->getArbitraryBorderVertex(arb);
            }
            
            if (config.debug) {
                std::cout << "minor model is full, choose arbitrary vertex...";
                Help::output(*((int*) &arb));
            }
            
            Vertex_Vec smash_verts;
            smash_verts.push_back(arb);
            order.addEnd(arb);
            
            Blob* b = new Blob();
            curr_mm->addBlob(b, arb);
            
            if (config.choose_arb_mode == 0) {
                curr_component->smashVertex(arb);
                
                //if component is empty after vertex deletion, delete
                if (curr_component->graph->getSize()) {
                    component_stack.push(curr_component);
                }
                else {
                    delete curr_component;
                }
            }
            else {
                std::vector<Component*> new_components = curr_component->smashPath(smash_verts, 1);
                
                for (std::vector<Component*>::const_iterator it_c = new_components.begin(); it_c != new_components.end(); it_c++) {    
                    component_stack.push(*it_c);
                }
                
                delete curr_component;
            }
        }
        
        int bar_width = 20;
        std::cout << "[";
        float progress = ((float) order.getSize() / (float) n_vertices)*20;
        for (int i = 0; i < bar_width; i++) {
            if (i < progress)
                std::cout << "#";
            else
                std::cout << " ";
        }
        std::cout << "]\r";
        std::cout.flush();
        //Help::output(order.getSize());
        //Help::output(component_stack.size());
    }
    
    //finish outputs
    Help::output();
    Help::output();
    Help::output("finished decomposition...");
    
    if (config.test) {
        std::cout << "tests were run... ";
        if (test_result) {
            Help::output("ok");
        }
        else {
            Help::output("failed");
        }
    }
    else {
        std::cout << "tests were not run... " << std::endl;
    }
    
    std::cout << "max Kt... ";
    Help::output(max_t);
    
    std::cout << "max stack size... ";
    Help::output(max_stack_size);
    
    if (order.getSize() < 100) {
        std::cout << "final order... " << std::endl;
        order.print();
    }
}

Vertex_Vec ShortestPathDecomposition::ShortestPathDecomposer::getOrder(){
    return order.getVertices();
}

int ShortestPathDecomposition::ShortestPathDecomposer::wColStupid(int r, int debug) {
    int max_w_reach = 0;
    Vertex_Vec graph_vertices = g->getVertices();
    //for every vertex u in graph
    for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
        int v_w_reach = 0;
        Path foundSmaller;
        //calculate r neighbourhood with bfs
        GraphWrapper* temp_g = g->bfsGraph(*it_v, r, true);
        
        Vertex_Vec neighbourhood_vertices = temp_g->getVertices();
        //for every of those neighbours
        for (Vertex_Vec::const_iterator it_v_2 = neighbourhood_vertices.begin(); it_v_2 != neighbourhood_vertices.end(); it_v_2++) {
            
            if (hasPathWCol(*temp_g, *it_v_2, *it_v, r)) {
                v_w_reach++;
                foundSmaller.addEnd(*it_v_2);
            }
            
        }
        
        if (v_w_reach > max_w_reach)
            max_w_reach = v_w_reach;
        
        delete temp_g;
    }
    
    return max_w_reach;
}

int ShortestPathDecomposition::ShortestPathDecomposer::wColBFS(int r, int debug) {
    int max = 0;
    Vertex_Vec graph_vertices = g->getVertices();
    std::map<Vertex_Descr, int> w_reach_size;
    
    for (Vertex_Vec::iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
        w_reach_size[*it_v] = 0;
    }
    
    //for every vertex u in graph
    for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
        Vertex_Vec w_reachable = wReachable(*it_v, r);
        for (Vertex_Vec::iterator it_reach = w_reachable.begin(); it_reach != w_reachable.end(); it_reach++) {
            increaseWReach(w_reach_size, *it_reach, max);
        }
    }
    
    return max;
}

int ShortestPathDecomposition::ShortestPathDecomposer::wColAsc(int r, int debug) {
    int max = 0;
    GraphWrapper* temp_g = new GraphWrapper(*g);
    Vertex_Vec graph_vertices = temp_g->getVertices();
    Vertex_Vec order_vertices = order.getVertices();
    std::map<Vertex_Descr, int> w_reach_size;
    for (Vertex_Vec::iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
        w_reach_size[*it_v] = 0;
    }
    
    //for every vertex u in graph
    for (Vertex_Vec::const_iterator it_order = order_vertices.begin(); it_order != order_vertices.end(); it_order++) {
        Vertex_Vec w_reachable = temp_g->bfsVertices(*it_order, r, true);
        for (Vertex_Vec::const_iterator it_reach = w_reachable.begin(); it_reach != w_reachable.end(); it_reach++) {
            increaseWReach(w_reach_size, *it_reach, max);
        }
        temp_g->removeVertex(*it_order, true);
    }
    
    delete temp_g;
    
    return max;
}

int ShortestPathDecomposition::ShortestPathDecomposer::wColDyn(int r, int debug) {
    int max = 0;
    std::map<Vertex_Descr, int> w_reach_size;
    std::map<Vertex_Descr,  Vertex_Vec> neighbourhood;
    std::map<std::pair<Vertex_Descr, Vertex_Descr>, Vertex_Descr> neighbourhood_min;
    Vertex_Vec graph_vertices = g->getVertices();
    
    
    for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v !=graph_vertices.end(); it_v++) {
        w_reach_size[*it_v] = 1;
    }
    
    for (int i = 1; i <= r; i++) {
        std::map<Vertex_Descr,  Vertex_Vec> neighbourhood_temp = neighbourhood;
        std::map<std::pair<Vertex_Descr, Vertex_Descr>, Vertex_Descr> neighbourhood_min_temp = neighbourhood_min;
        
        if (i == 1) {
            for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
                int it_v_order_pos = order.getPosition(*it_v);
                Vertex_Vec neighbours = g->neighbours(*it_v, true);
                for (Vertex_Vec::const_iterator it_neighbour = neighbours.begin(); it_neighbour != neighbours.end(); it_neighbour++) {
                    int it_neighbour_order_pos = order.getPosition(*it_neighbour);
                    
                    if (it_v_order_pos < it_neighbour_order_pos) {
                        increaseWReach(w_reach_size, *it_neighbour, max);
                        neighbourhood_temp[*it_v].push_back(*it_neighbour);
                        neighbourhood_min_temp[std::make_pair(*it_v, *it_neighbour)] = *it_v;
                    }
                    else {
                        neighbourhood_temp[*it_v].push_back(*it_neighbour);
                        neighbourhood_min_temp[std::make_pair(*it_v, *it_neighbour)] = *it_neighbour;
                    }
                }
            }
        }
        
        else {
            //for all vertices
            for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
                int it_v_order_pos = order.getPosition(*it_v);
                Vertex_Vec* it_v_neighbourhood_temp = &neighbourhood_temp[*it_v];
                Vertex_Vec neighbours = g->neighbours(*it_v, true);

                for (Vertex_Vec::const_iterator it_neighbour = neighbours.begin(); it_neighbour != neighbours.end(); it_neighbour++) {
    
                    Vertex_Vec new_neighbourhood = neighbourhood[*it_neighbour];
    
                    for (Vertex_Vec::const_iterator it_n = new_neighbourhood.begin(); it_n != new_neighbourhood.end(); it_n++) {
                        //int it_n_order_pos = order.getPosition(*it_n);
    
                        if (std::find(it_v_neighbourhood_temp->begin(), it_v_neighbourhood_temp->end(), *it_n) == it_v_neighbourhood_temp->end()) {
                            Vertex_Descr path_neighbour_n_min = neighbourhood_min[std::make_pair(*it_neighbour, *it_n)];
                            int path_neighbour_n_order_min = order.getPosition(path_neighbour_n_min);
                            if (it_v_order_pos < path_neighbour_n_order_min) {
                                increaseWReach(w_reach_size, *it_n, max);
                                it_v_neighbourhood_temp->push_back(*it_n);
                                neighbourhood_min_temp[std::make_pair(*it_v, *it_n)] = *it_v;
                            }
                            else { 
                                if (*it_v != *it_n){
                                    it_v_neighbourhood_temp->push_back(*it_n);
                                    neighbourhood_min_temp[std::make_pair(*it_v, *it_n)] = path_neighbour_n_min;
                                }
                            }
                        }
                        else {
                            Vertex_Descr path_v_n_old_min = neighbourhood_min_temp[std::make_pair(*it_v, *it_n)];
                            int path_v_n_old_order_min = order.getPosition(path_v_n_old_min);
                            Vertex_Descr path_v_n_new_min = neighbourhood_min_temp[std::make_pair(*it_neighbour, *it_n)];
                            int path_v_n_new_order_min = order.getPosition(path_v_n_new_min);
                            
                            if (path_v_n_old_order_min < it_v_order_pos && it_v_order_pos < path_v_n_new_order_min) {
                                increaseWReach(w_reach_size, *it_n, max);
                            }
                            
                            if (path_v_n_new_order_min > path_v_n_old_order_min) {
                                if (path_v_n_new_order_min < it_v_order_pos){
                                    neighbourhood_min_temp[std::make_pair(*it_v, *it_n)] = path_v_n_new_min;
                                }
                                else{
                                    neighbourhood_min_temp[std::make_pair(*it_v, *it_n)] = *it_v;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        neighbourhood = neighbourhood_temp;
        neighbourhood_min = neighbourhood_min_temp;
        
        //printNeighbours(w_reach_size, neighbourhood, neighbourhood_min, graph_vertices);
    }
    
    return max;
}

int ShortestPathDecomposition::ShortestPathDecomposer::wCol(int wcol_mode, int r, int debug) {
    int wcol = 0;
    switch(wcol_mode) {
        case 0:
            wcol = wColStupid(r, debug);
        case 1:
            wcol = wColBFS(r, debug);
        case 2:
            wcol = wColAsc(r, debug);
        case 3:
            wcol = wColDyn(r, debug);
    }
    
    return wcol;
}

void ShortestPathDecomposition::ShortestPathDecomposer::increaseWReach(std::map<Vertex_Descr, int> &w_reach_size, Vertex_Descr v, int &max) {
    int w_reach_size_temp = w_reach_size[v];
    w_reach_size[v] = w_reach_size_temp+1;
    if (w_reach_size_temp+1>max)
        max = w_reach_size_temp+1;
}

void ShortestPathDecomposition::ShortestPathDecomposer::printNeighbours(std::map<Vertex_Descr, int> &w_reach_size,
    std::map<Vertex_Descr,  Vertex_Vec> &neighbourhood,
    std::map<std::pair<Vertex_Descr, Vertex_Descr>, Vertex_Descr> &neighbourhood_min,
    Vertex_Vec &graph_vertices) {
    for (Vertex_Vec::const_iterator it_v = graph_vertices.begin(); it_v != graph_vertices.end(); it_v++) {
        std::cout << *((int*) &(*it_v)) << ": " << std::endl;
        Vertex_Vec it_v_neighbourhood = neighbourhood[*it_v];
        for (Vertex_Vec::const_iterator it_n = it_v_neighbourhood.begin(); it_n != it_v_neighbourhood.end(); it_n++) {
            std::cout << "(" << *((int*) &(*it_n)) << "," << *((int*) &neighbourhood_min[std::make_pair(*it_v, *it_n)]) << ") ";
        }
        std::cout << "" << std::endl;
    }
    Help::output();
}

Vertex_Vec ShortestPathDecomposition::ShortestPathDecomposer::wReachable(Vertex_Descr source, int r) {
    int source_order_pos = order.getPosition(source);
    Vertex_Vec ret;
    ret.clear();
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, bool> colors;
    std::map<Vertex_Descr, int> distances;
    colors[source] = true;
    distances[source] = 0;
    queue.push(source);
    ret.push_back(source);
    
    while (!queue.empty()) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        Vertex_Vec adj = g->neighbours(curr, true);
        int curr_distance = distances[curr];
        
        for (Vertex_Vec::iterator it_adj = adj.begin(); it_adj != adj.end(); it_adj++) {
            if (colors.find(*it_adj) == colors.end()) {
                int it_adj_order_pos = order.getPosition(*it_adj);
                if ((source_order_pos < it_adj_order_pos) && curr_distance + 1 <= r) {
                    colors[*it_adj] = true;
                    distances[*it_adj] = curr_distance + 1;
                    queue.push(*it_adj);
                    ret.push_back(*it_adj);
                }
            }
        }
        distances.erase(curr);
    }
    
    return ret;
}

bool ShortestPathDecomposition::ShortestPathDecomposer::hasPathWCol(GraphWrapper &graph, Vertex_Descr source, Vertex_Descr target, int r) {
    //find path to u without
    int source_order_pos = order.getPosition(source);
    if (source == target)
        return true;
    
    std::queue<Vertex_Descr> queue;
    std::map<Vertex_Descr, bool> colors;
    std::map<Vertex_Descr, int> distances;
    colors[source] = 0;
    distances[source] = 0;
    queue.push(source);
    
    while (!queue.empty()) {
        Vertex_Descr curr = queue.front();
        queue.pop();
        Vertex_Vec adj = graph.neighbours(curr, true);
        int curr_distance = distances[curr];
        
        for (Vertex_Vec::iterator it_adj = adj.begin(); it_adj != adj.end(); it_adj++) {
            if (colors.find(*it_adj) == colors.end()) {
                int it_adj_order_pos = order.getPosition(*it_adj);
                if (!(it_adj_order_pos < source_order_pos) && curr_distance + 1 <= r) {
                    if (*it_adj == target)
                        return true;
                    colors[*it_adj] = true;
                    distances[*it_adj] = curr_distance + 1;
                    queue.push(*it_adj);
                }
            }
        }
        
        distances.erase(curr);
    }
    
    return false;
}

