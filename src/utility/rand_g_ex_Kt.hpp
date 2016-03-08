#ifndef RAND_G_EX_KT_H
#define RAND_G_EX_KT_H

#include <utility>

#include "../main/DSGraph.hpp"

/**
 * @brief Create a random graph with n vertices, **per expectation** exluding Kt
 * 
 * For each pair of vertices u, v with u != v, add edge {u, v} if sample from
 * Bernoulli~(1/n * log_10 t * t) is 1.
 *
 * The set of vertices is {0, ..., n-1}.
 */
namespace rand_g_ex_Kt{
    /** Return pair (DSGraph, num_edges) */
    std::pair<DSGraph, unsigned int> create(unsigned int n, unsigned int t);
}

#endif
