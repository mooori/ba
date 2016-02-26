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
    // (taken from cppreference.com: std::uniform_real_distribution)
    // for threshold
    std::random_device rd_t;
    std::mt19937 gen_t(rd_t());
    std::uniform_real_distribution<float> dis_t(0, 1);

    // ...for edges
    std::random_device rd_e;
    std::mt19937 gen_e(rd_e());
    std::uniform_real_distribution<float> dis_e(0, 1);

    // add edges
    for(unsigned int i = 0; i < n; ++i) {
        float threshold = dis_t(gen_t);
        for(unsigned int j = 0; j < n; ++j) {
            if(i == j) { continue; }    // no loops in graph
            if(dis_e(gen_e) > threshold && 
                        !dsg.in_adj_list_I(i, j)) {     // no dupl edges
                dsg.add_IEdge(IEdge(i, j));
            }
        }
    }

    return dsg;
}
