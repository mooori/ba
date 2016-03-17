#include <cmath>
#include <random>
#include <utility>

#include "rand_g_ex_Kt.hpp"
#include "../main/DSGraph.hpp"
#include "../types/dstypes.hpp"

std::pair<DSGraph, unsigned int> 
        rand_g_ex_Kt::create(unsigned int n, unsigned int t) {
    DSGraph dsg;

    // add vertices
    for(unsigned int i = 0; i < n; ++i) {
        dsg.add_IVertex(i);
    }

    float p_edge = (1/float(n)) * std::sqrt(std::log(t)) * t;
    unsigned int nedges = 0;

    // create random number generator for float [0,1]
    // (taken from cppreference.com: std::uniform_real_distribution)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(p_edge);

    // add edges
    for(unsigned int i = 0; i < n; ++i) {
        for(unsigned int j = 0; j < n; ++j) {
            if(i == j) { continue; }    // no loops in graph
            bool b = dis(gen);
            if(b && !dsg.in_adj_list_I(i, j)) {    // no dupl edges
                dsg.add_IEdge(IEdge(i, j));
                ++nedges;
            }
        }
    }
    
    return std::pair<DSGraph, unsigned int>(dsg, nedges);
}
