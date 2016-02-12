#ifndef DSGRAPH_H
#define DSGRAPH_H

#include <list>
#include <map>
#include <set>
#include <utility>

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
    BGraph::vertices_size_type num_vertices() const;

    /**
     * Get BVertex iterator
     * @returns std::pair of vertex iterators, first pointing to beginning of
     *     vertex array, second past the end, @see BGL quick tour
     */
    std::pair<BVertex_it, BVertex_it> vertices() const; 

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
    void add_IVertices(std::list<IVertex>& vids);

    /**
     * Add edge between IVertices
     * @param e edge to be added, for type IEdge @see dstypes.hpp
     * @throws std::out_of_range("IVertex not in G") if any of input
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
    void add_IEdges(std::list<IEdge>& es); 

    /**
     * Get BVertex by IVertex
     * @throws std::out_of_range if vertex not present
     */
    BVertex get_BVertex(IVertex vid) const;

    /**
     * Get IVertex by BVertex
     * @throws std::out_of_range if vertex not present
     */
    IVertex get_IVertex(BVertex bvid) const;

    /**
     * Get IEdge by BEdge
     * @throws @see get_IVertex
     */
    IEdge get_IEdge(BEdge be) const;

    /**
     * Get BVertices adjacent to BVertex
     * @param bvid BVertex of which adj vertices are requested
     * @returns std::set of adjacent BVertices
     * @throws std::runtime_error if a bvid appears more than once in adj list
     */
    std::set<BVertex> get_adj_BVertices(BVertex bvid) const;

    /**
     * Get IVertices adjacent to IVertex
     * @param vid IVertex of which adj vertices are requested
     * @returns std::set of adjacent IVertices
     * @throws @see get_IVertex @see get_adj_BVertices
     */
    std::set<IVertex> get_adj_IVertices(IVertex vid) const;

    /**
     * Remove BVertex
     * Side effects: clear v, remove v from maps
     * @throws @see maps_remove_BVertex
     */
    void remove_BVertex(BVertex bvid);

    /**
     * Remove IVertex, just a wrapper for @see remove_BVertex
     * @throws @see get_BVertex, @see remove_BVertex
     */
    void remove_IVertex(IVertex vid);

    /**
     * Get set of Graph's BVertices
     * @throws std::runtime_error, if inserting vertex in set fails, since
     *     vertices must be unique.
     */
    std::set<BVertex> get_set_BVertices() const;

    /**
     * Get set of Graph's IVertices
     * @throws std::runtime_error if inserting vertex in set fails, since
     *     vertices must be unique.
     */
    std::set<IVertex> get_set_IVertices() const;

    /** Check if DSGraph contains BVertex */
    bool contains_BVertex(const BVertex bvid) const;

    /** Check if DSGraph contains IVertex */
    bool contains_IVertex(const IVertex vid) const;

    /** Return true if bvid2 is in adj list of bvid1 */
    bool in_adj_list_B(const BVertex bvid1, const BVertex bvid2) const;

    /** Return true if vid2 is in adj list of vid1 */
    bool in_adj_list_I(const IVertex vid1, const IVertex vid2) const;

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

    /** map[bid] = oid, @see map_oid_bid */
    map_v_B2I_t map_v_B2I;

    /**
     * Remove BVertex from both maps
     * @throws std::runtime_error if BVertex not in maps
     */
    void maps_remove_BVertex(BVertex bvid);
};

#endif
