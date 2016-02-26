#include <random>

#include "rand_g.hpp"
#include "../main/DSGraph.hpp"
#include "../types/dstypes.hpp"

DSGraph rand_g::create(unsigned int n) {
    DSGraph dsg;

    // add vertices
    for(unsigned int i = 0; i < n; ++i) {
        dsg.add_IVertex(i);
    }

    // create random number generator for unsigned int [0,1]
    // (taken from cppreference.com: std::uniform_int_distribution)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, 1);

    // add edges
    for(unsigned int i = 0; i < n; ++i) {
        for(unsigned int j = 0; j < n; ++j) {
            if(i == j) { continue; }    // no loops in graph
            if(dis(gen) && !dsg.in_adj_list_I(i, j)) {     // no dupl edges
                dsg.add_IEdge(IEdge(i, j));
            }
        }
    }

    return dsg;
}
