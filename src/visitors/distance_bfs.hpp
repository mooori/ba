#ifndef DISTANCEBFS_H
#define DISTANCEBFS_H

/**
 * @class distance_bfs distance_bfs.h
 * @brief Visitor used to determine vertex's (called `src`) eccentricity
 *
 * Using tree_edge visitor to be sure distance from source increases along each
 *     edge.
 *
 * @note Example usage: @see Distance.hpp, which uses eccs to calculate radius
 *     and diameter.
 */

#include <algorithm>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

template<typename Container>
class distance_bfs : public boost::default_bfs_visitor {
public:
    /**
     * Constructor.
     * @param d Container of length num_vertices(g), initiated with d[src] = 0
     *     and all other values of d set to -1
     */
    distance_bfs(Container& ds, int& dm) : dists_src(ds), max_dist(dm) { }

    template<typename Edge, typename Graph>
    void tree_edge(Edge e, const Graph& g) const {
        typename boost::graph_traits<Graph>::vertex_descriptor u, v;
        u = boost::source(e, g), v = boost::target(e, g);
        dists_src[v] = dists_src[u] + 1;
        max_dist = std::max(max_dist, dists_src[v]);
    }

private:
    /** dists_src[i] holds i's dist from this bfs's src */
    Container& dists_src;

    /** max length of path from bfs's src, to become src's eccentricity */
    int& max_dist;
};

#endif
