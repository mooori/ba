#pragma once

#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "Common.h"
#include "Order.hpp"
#include "Component.hpp"

namespace ShortestPathDecomposition {
    class ShortestPathDecomposer {
    public:        
        ShortestPathDecomposer();
        
        ~ShortestPathDecomposer();
        
        struct Config {
            bool test;
            int debug;
            bool viz;
            int choose_arb_mode;
        };
        
        void init(Component* c_in);
        
        void init(Graph &g_in);
        
        void decompose(Config config);
        
        Vertex_Vec getOrder();
        
        int wCol(int wcol_mode, int r, int debug);
        
    private:
        void increaseWReach(std::map<Vertex_Descr, int> &w_reach_size, Vertex_Descr v, int &max);
        
        void printNeighbours(std::map<Vertex_Descr, int> &w_reach_size,
        std::map<Vertex_Descr,  Vertex_Vec> &neighbourhood,
        std::map<std::pair<Vertex_Descr, Vertex_Descr>, Vertex_Descr> &neighbourhood_min,
        Vertex_Vec &graph_vertices);
        
        Vertex_Vec wReachable(Vertex_Descr source, int r);
        
        bool hasPathWCol(GraphWrapper &graph, Vertex_Descr source, Vertex_Descr target, int r);
        
        int wColStupid(int r, int debug);
        
        int wColBFS(int r, int debug);
        
        int wColAsc(int r, int debug);
        
        int wColDyn(int r, int debug);
        
        Component* c;
        GraphWrapper* g;
        Order order;
        bool test_result;
    };
}
