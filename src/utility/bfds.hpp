#ifndef BFDS_H
#define BFDS_H

#include <set>
#include <utility>

#include "helpers.hpp"
#include "Parser.hpp"
#include "../main/DSGraph.hpp"
#include "../main/SubsetGen.hpp"

/**
 * @brief Brute force calculation of minimum dominating set.
 *
 * For k <= V(G)
 *     For size k subset of V(G)
 *         if subset is ds: return subset
 */

namespace bfds {
    /** get min ds */
    std::set<IVertex> get_min_ds(const DSGraph& dsg);
}

#endif
