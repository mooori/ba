#ifndef RAND_G_H
#define RAND_G_H

#include "../main/DSGraph.hpp"

/**
 * @brief Create a random graph with n vertices.
 *
 * For each pair of vertices u, v with u != v, add edge {u, v} if random
 * value in [0,1] is greater than a random threshold in [0,1]. Calculate
 * this threshold new for each vertex in outer loop.
 *
 * The set of vertices is {0, ..., n-1}.
 */
namespace rand_g {
    /** Create random graph with n vertices */
    DSGraph create(unsigned int n);

}    // namespace

#endif
