#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include "types/distance_info.hpp"
#include "utility/constants.hpp"
#include "visitors/distance_bfs.hpp"

/**
 * @class Distance
 * @brief Calculate radius and diameter for a graph.
 *
 * Is supposed to be used on connected components only.
 * Makes use of @see distance_bfs.hpp
 */

template<class Graph>
class Distance {
public:
    
    Distance() { }

    /**
     * Get g's distance info.
     * @param g Graph
     * @returns distance_info struct, @see distance_info.hpp
     */
    distance_info get_info(Graph& g) {
        const int num_verts = boost::num_vertices(g);
        std::vector<int> eccs(num_verts,
                constants::distance_init); //eccentricities
        distance_info di;

        // starting from each vertex do bfs to get it's eccentricity
        // upd graph's radius and diameter, if necessary
        for(int i = 0; i < num_verts; ++i) {
            std::vector<int> dists_src(num_verts, constants::distance_init);
            dists_src[i] = 0;
            int ecc_i = constants::distance_init;
            breadth_first_search(g, i, visitor(
                    distance_bfs< std::vector<int> >(dists_src, ecc_i)));
            eccs[i] = ecc_i;
            di.radius = std::min(di.radius, ecc_i);
            di.diameter = std::max(di.diameter, ecc_i);
        }

        return di;
    }
};

#endif
