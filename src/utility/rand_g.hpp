#ifndef RAND_G_H
#define RAND_G_H

#include "../main/DSGraph.hpp"

/**
 * @brief Create a random graph with n vertices.
 *
 * For each pair of vertices u, v with u != v, add edge {u, v} if random
 * int in [0, 1] equals one.
 *
 * The set of vertices is {0, ..., n-1}.
 */
namespace rand_g {
    /** Create random graph with n vertices */
    DSGraph create(unsigned int n);

}    // namespace

#endif
