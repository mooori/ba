#ifndef DSTYPES_H
#define DSTYPES_H

#include <map>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

/** @brief Definition of some commonly used types related to Graph object
 * B relates to Boost Graph Library
 * I relates to original input
 */

/** Boost Graph */
typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS>
        BGraph;

typedef BGraph::vertex_descriptor BVertex;
typedef BGraph::edge_descriptor BEdge;

typedef BGraph::vertex_iterator BVertex_it;
typedef BGraph::edge_iterator BEdge_it;

typedef typename boost::graph_traits<BGraph>::adjacency_iterator
        B_adjacency_it;

typedef unsigned int IVertex;
typedef std::pair<IVertex, IVertex> IEdge;

#endif
