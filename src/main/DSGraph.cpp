#include <map>
#include <set>
#include <stdexcept>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "DSGraph.hpp"
#include "../types/dstypes.hpp"

DSGraph::DSGraph()
    : G(new BGraph())
    , map_v_I2B(map_v_I2B_t())
    , map_v_B2I(map_v_B2I_t())
    , color_H(new std::set<BVertex>())
    { }

DSGraph::~DSGraph() {
    delete G;
    delete color_H;
}

BGraph::vertices_size_type DSGraph::num_vertices() const {
    return boost::num_vertices(*(this->G));
}

std::pair<BVertex_it, BVertex_it> DSGraph::vertices() const {
    return boost::vertices(*(this->G));
}

std::pair<BEdge_it, BEdge_it> DSGraph:: edges() {
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

void DSGraph::add_IVertices(std::list<IVertex>& vids) {
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

void DSGraph::add_IEdges(std::list<IEdge>& es) {
    for(std::list<IEdge>::iterator it = es.begin(); it != es.end(); ++it) {
        this->add_IEdge(*it);
    }
    return;
}

BVertex DSGraph::get_BVertex(IVertex vid) const {
    map_v_I2B_t::const_iterator it = this->map_v_I2B.find(vid);
    if(it == map_v_I2B.end()) {
        throw std::out_of_range("IVertex not in G");
    }
    return it->second;
}

IVertex DSGraph::get_IVertex(BVertex bvid) const {
    map_v_B2I_t::const_iterator it = this->map_v_B2I.find(bvid);
    if(it == map_v_B2I.end()) {
        throw std::out_of_range("BVertex not in G");
    }
    return it->second;
}

IEdge DSGraph::get_IEdge(BEdge be) const {
    return IEdge(this->get_IVertex(boost::source(be, *(this->G))),
            this->get_IVertex(boost::target(be, *(this->G))));
}

std::set<BVertex> DSGraph::get_adj_BVertices(BVertex bvid) const {
    std::pair<B_adj_it, B_adj_it> adj_it = 
            boost::adjacent_vertices(bvid, *(this->G));
    std::set<BVertex> adj_verts;
    
    for( ; adj_it.first != adj_it.second; ++adj_it.first) {
        std::pair< std::set<BVertex>::iterator, bool > res =
                adj_verts.insert(*adj_it.first);
        if(!res.second) {
            throw std::runtime_error("duplicate adjacent vertices");
        }
    }

    return adj_verts;
}

std::set<IVertex> DSGraph::get_adj_IVertices(IVertex vid) const {
    std::pair<B_adj_it, B_adj_it> adj_it =
            boost::adjacent_vertices(this->get_BVertex(vid), *(this->G));
    std::set<IVertex> adj_verts;

    for( ; adj_it.first != adj_it.second; ++adj_it.first) {
        std::pair< std::set<IVertex>::iterator, bool > res =
                adj_verts.insert(this->get_IVertex(*adj_it.first));
        if(!res.second) {
            throw std::runtime_error("duplicate adjacent vertices");
        }
    }

    return adj_verts;
}

void DSGraph::remove_BVertex(BVertex bvid) {
    // boost::clear_vertex just dumps core if bvid not in G
    // not really helpful for error handling...
    boost::clear_vertex(bvid, *(this->G));
    this->maps_remove_BVertex(bvid);
    boost::remove_vertex(bvid, *(this->G));
    return;
}

void DSGraph::remove_IVertex(IVertex vid) {
    this->remove_BVertex(this->get_BVertex(vid));
    return;
}

std::set<BVertex> DSGraph::get_set_BVertices() const {
    // iterate over vertices and insert each to set
    std::set<BVertex> s;
    std::pair<BVertex_it, BVertex_it> v_it = this->vertices();
    for( ; v_it.first != v_it.second; ++v_it.first) {
        std::pair<std::set<BVertex>::iterator, bool> ins =
                s.insert(*v_it.first);
        if(!ins.second) {
            throw std::runtime_error("failed to insert BVertex in set");
        }
    }
    return s;
}

bool DSGraph::contains_BVertex(const BVertex bvid) const {
    map_v_B2I_t::const_iterator it = this->map_v_B2I.find(bvid);
    return it != this->map_v_B2I.end();
}

bool DSGraph::contains_IVertex(const IVertex vid) const {
    map_v_I2B_t::const_iterator it = this->map_v_I2B.find(vid);
    return it != this->map_v_I2B.end();
}

bool DSGraph::in_adj_list_B(const BVertex bvid1, const BVertex bvid2) const {
    std::set<BVertex> N1 = this->get_adj_BVertices(bvid1);
    std::set<BVertex>::const_iterator it = N1.find(bvid2);
    return it != N1.end();
}

bool DSGraph::in_adj_list_I(const IVertex vid1, const IVertex vid2) const {
    std::set<IVertex> N1 = this->get_adj_IVertices(vid1);
    std::set<IVertex>::const_iterator it = N1.find(vid2);
    return it != N1.end();
}

void DSGraph::maps_remove_BVertex(BVertex bvid) {
    IVertex vid = this->get_IVertex(bvid);
    if(this->map_v_I2B.erase(vid) != 1) {
        throw std::runtime_error("IVertex not exactly once in map_v_I2B");
    }
    if(this->map_v_B2I.erase(bvid) != 1) {
        throw std::runtime_error("BVertex not exactly once in map_v_B2I");
    }
    return;
}
