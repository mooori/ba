#ifndef DSGRAPH_H
#define DSGRAPH_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../types/dstypes.hpp"

/**
 * @brief Hold BGL Graph and graph properties and provide methods on them.
 *
 * BGL "documentation" often not very helpful, in many cases don't know
 * what actually happens. So instead of using BGL graph properties, hold
 * properties in this struct, gives you more control over them.
 */
class DSGraph {
public:
    /** Constructor, initialisation with empty default objects */
    DSGraph();

    /** Destructor, deleting ptr to G */
    ~DSGraph();

    /** Boost Graph */
    BGraph* G;

    /**
     * Get number of G's vertices
     */
    BGraph::vertices_size_type num_vertices();

    /**
     * Get BVertex iterator
     * @returns std::pair of vertex iterators, first pointing to beginning of
     *     vertex array, second past the end, @see BGL quick tour
     */
    std::pair<BVertex_it, BVertex_it> vertices();

    /**
     * Get BEdge iterator
     * @returns std::pair of edge iterators, first pointing to beginning of
     *     edge array, second pointing past the end, @see BGL quick tour
     */
    std::pair<BEdge_it, BEdge_it> edges();

    /**
     * Add vertex to graph and upd id maps
     * @param IVertex to be added to graph
     * @throws std::runtime_error if inserting in vertex id map(s) failed,
     *     e.g. because IVertex already exists.
     */
    void add_IVertex(IVertex vid);

    /**
     * Add vertices (calling add_IVertex for each v in list)
     * @param vids list of IVertices to add
     * @throws @see add_IVertex
     */
    void add_IVertices(std::list<IVertex> vids);

    /**
     * Add edge between IVertices
     * @param e edge to be added, for type IEdge @see dstypes.hpp
     * @throws std::runtime_error("IVertex not in G") if any of input
     *     vertices not in G, @see getBVertex
     * @throws std::runtime_error("duplicate edges not allowed")
     *     if boost::add_vertex returns bool flag FALSE (edge already in G).
     */
    void add_IEdge(IEdge e);

    /**
     * Add edges (calling add_IEdge for each e in list)
     * @param es list of IEdges to add
     * @throws @see add_IEdge
     */
    void add_IEdges(std::list<IEdge> es);

    /**
     * Get BVertex by IVertex
     * @throws std::out_of_range if vertex not present
     */
    BVertex get_BVertex(IVertex vid);

    /**
     * Get IVertex by BVertex
     * @throws std::out_of_range if vertex not present
     */
    IVertex get_IVertex(BVertex bvid);

    /**
     * Get IEdge by BEdge
     * @throws @see get_IVertex
     */
    IEdge get_IEdge(BEdge be);

private:
    /** type of map from input to boost vertex */
    typedef std::map<IVertex, BVertex> map_v_I2B_t;

    /** type of map from boost to input vertex */
    typedef std::map<BVertex, IVertex> map_v_B2I_t;

    /**
     * map[oid] = bid
     * oid: id given to vertex in graph file
     * bid: id assigned to vertex in G (by BGL)
     */
    map_v_I2B_t map_v_I2B;

    /**
     * map[bid] = oid, @see map_oid_bid
     */
    map_v_B2I_t map_v_B2I;
};

DSGraph::DSGraph()
    : G(new BGraph())
    , map_v_I2B(map_v_I2B_t())
    , map_v_B2I(map_v_B2I_t())
    { }

DSGraph::~DSGraph() {
    delete G;
}

BGraph::vertices_size_type DSGraph::num_vertices() {
    return boost::num_vertices(*(this->G));
}

std::pair<BVertex_it, BVertex_it> DSGraph::vertices() {
    return boost::vertices(*(this->G));
}

std::pair<BEdge_it, BEdge_it> DSGraph::edges() {
    return boost::edges(*(this->G));
}

void DSGraph::add_IVertex(IVertex vid) {
    BVertex bvid = boost::add_vertex(*(this->G));

    // insert to map_v_I2B
    std::pair<map_v_I2B_t::iterator, bool> res_I2B =
            this->map_v_I2B.insert(std::pair<IVertex, BVertex>(vid, bvid));
    if(!res_I2B.second) {
        // probably sth like key already in map, check input graph
        throw std::runtime_error("failed to insert in map_v_I2B");
    }

    // insert to map_v_B2I
    std::pair<map_v_B2I_t::iterator, bool> res_B2I =
            this->map_v_B2I.insert(std::pair<BVertex, IVertex>(bvid, vid));
    if(!res_B2I.second) {
        throw std::runtime_error("failed to insert in map_v_B2I");
    }

    return;
}

void DSGraph::add_IVertices(std::list<IVertex> vids) {
    for(std::list<IVertex>::iterator it = vids.begin(); it != vids.end();
            ++it) {
        this->add_IVertex(*it);
    }
    return;
}

void DSGraph::add_IEdge(IEdge e) {
    BVertex bvid1, bvid2;
    bvid1 = this->get_BVertex(e.first);
    bvid2 = this->get_BVertex(e.second);
    std::pair<BEdge, bool> res = boost::add_edge(bvid1, bvid2, *(this->G));
    if(res.second == false) {
        throw std::runtime_error("duplicate edges not allowed");
    }
    return;
}

void DSGraph::add_IEdges(std::list<IEdge> es) {
    for(std::list<IEdge>::iterator it = es.begin(); it != es.end(); ++it) {
        this->add_IEdge(*it);
    }
    return;
}

BVertex DSGraph::get_BVertex(IVertex vid) {
    map_v_I2B_t::iterator it = this->map_v_I2B.find(vid);
    if(it == map_v_I2B.end()) { throw std::out_of_range("IVertex not in G"); }
    return it->second;
}

IVertex DSGraph::get_IVertex(BVertex bvid) {
    map_v_B2I_t::iterator it = this->map_v_B2I.find(bvid);
    if(it == map_v_B2I.end()) { throw std::out_of_range("BVertex not in G"); }
    return it->second;
}

IEdge DSGraph::get_IEdge(BEdge be) {
    return IEdge(this->get_IVertex(boost::source(be, *(this->G))),
            this->get_IVertex(boost::target(be, *(this->G))));
}

#endif
