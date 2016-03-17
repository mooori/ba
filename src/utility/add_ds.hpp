#ifndef ADD_DS_H
#define ADD_DS_H

#include "../main/DSGraph.hpp"

/**
 * @brief Add dom set of size k to a graph. 
 *
 * Randomly pick k vertices to be dom set.
 * For each vertex:
 *     - guess to how many verts from dom set adjacent
 *     - guess which ones
 *     - add corresponding edges
 *
 * Vertices **not** not required to be [0,...,n] (but can, of course).
 */

namespace add_ds {

    /** Add dom set of size k to dsg */
    void add_ds(DSGraph& dsg, unsigned int k);

}    // namesapce

#endif
