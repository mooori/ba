#ifndef DSTYPES_H
#define DSTYPES_H

#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

/** @brief Definition of some commonly used types related to Graph object */

/**
 * @note Custom interior vertex properties
 * Custom interior vertex properties are added according to example in 
 * libs/graph/example/interior_property_map.cpp
 * This was the only hint I found on how to do it.
 */

/** interior vertex prop: original_ida*/
enum vertex_original_id_t { vertex_original_id };
namespace boost {
    BOOST_INSTALL_PROPERTY(vertex, original_id);
}

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
        boost::property<vertex_original_id_t, int> > DSGraph;

typedef unsigned int DSVertex;
typedef std::pair<int, int> DSEdge;

typedef typename boost::graph_traits<DSGraph>::edge_iterator DS_edge_it_t;
typedef typename boost::graph_traits<DSGraph>::vertex_iterator DS_vertex_it_t;

typedef typename boost::graph_traits<DSGraph>::adjacency_iterator
        DS_adjacency_it;

typedef typename boost::property_map<DSGraph, vertex_original_id_t>::type
        DSProp_Acc_original_id;

#endif
