#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include "DSGraph.hpp"
#include "../types/distance_info.hpp"
#include "../types/dstypes.hpp"
#include "../utility/constants.hpp"
#include "../visitors/distance_bfs.hpp"

/**
 * @class Distance
 * @brief Calculate radius and diameter for a graph.
 *
 * Is supposed to be used on connected components only.
 * Makes use of @see distance_bfs.hpp
 */

class Distance {
public:
    
    Distance() { }

    /**
     * Get g's distance info.
     * @param g Graph
     * @returns distance_info struct, @see distance_info.hpp
     */
    distance_info get_info(DSGraph& dsg) {
        distance_info di;

        // starting from each vertex do bfs to get it's eccentricity
        // upd graph's radius and diameter, if necessary
        for(std::pair<BVertex_it, BVertex_it> vit = dsg.vertices();
                vit.first != vit.second; ++vit.first) {
            std::map<BVertex, int> dists_src;
            dists_src[*vit.first] = 0;
            int ecc_i = constants::distance_init;
            boost::breadth_first_search(*(dsg.G), *vit.first, boost::visitor(
                    distance_bfs< std::map<BVertex, int> >(dists_src, ecc_i)));
            di.radius = std::min(di.radius, ecc_i);
            di.diameter = std::max(di.diameter, ecc_i);
        }

        return di;
    }
};

#endif
