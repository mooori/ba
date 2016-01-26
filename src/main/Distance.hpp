#ifndef DISTANCE_H
#define DISTANCE_H

#include <algorithm>
#include <map>
#include <vector>

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

        // edge cases: empty graph or just a single vertex
        // (no other edge cases, as Distance supposed to work on connected
        //  componentes, only!)
        if(dsg.num_vertices() == 0 || dsg.num_vertices() == 1) {
            di.radius = 0; di.diameter = 0;
            return di;
        }

        // make boost map, mapping vertex id's to ints in 
        // [0, dsg.num_vertices()] (required for bfs)
        typedef std::map<BVertex, int> vid_map_t;
        vid_map_t vid_map;
        std::pair<BVertex_it, BVertex_it> vit = dsg.vertices();
        boost::associative_property_map< std::map<BVertex, int> >
                boost_vid_map(vid_map);
        int i = 0;
        for( ; vit.first != vit.second; ++vit.first) {
            std::pair<vid_map_t::iterator, bool> res =
                    vid_map.insert(std::pair<BVertex, int>(*vit.first, i));
            if(!res.second) {
                throw std::runtime_error("failed insert in vertex index map");
            }
            ++i;
        }

        // starting from each vertex do bfs to get it's eccentricity
        // upd graph's radius and diameter, if necessary
        for(std::pair<BVertex_it, BVertex_it> vit = dsg.vertices();
                vit.first != vit.second; ++vit.first) {
            std::map<BVertex, int> dists_src;
            dists_src[*vit.first] = 0;
            int ecc_i = constants::distance_init;
            boost::breadth_first_search(*(dsg.G), *vit.first, 
                    boost::visitor(distance_bfs< std::map<BVertex, int> >
                    (dists_src, ecc_i)).    // bgl_named_param notation
                    vertex_index_map(boost_vid_map));
            di.radius = std::min(di.radius, ecc_i);
            di.diameter = std::max(di.diameter, ecc_i);
        }

        return di;
    }
};

#endif
