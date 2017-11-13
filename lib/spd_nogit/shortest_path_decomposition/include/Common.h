#pragma once

#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

namespace b = boost;

typedef boost::adjacency_list<boost::listS, boost::listS, b::undirectedS, b::property<b::vertex_index_t, int > > Graph;
typedef b::graph_traits<Graph>::vertex_descriptor Vertex_Descr;
typedef b::graph_traits<Graph>::vertex_iterator Vertex_Iter;
typedef b::graph_traits<Graph>::adjacency_iterator Adj_Iter;
typedef b::graph_traits<Graph>::edge_iterator Edge_Iter;
typedef std::pair<Vertex_Descr, Vertex_Descr> Edge;
typedef std::vector<Vertex_Descr> Vertex_Vec;
typedef std::map<Vertex_Descr, Vertex_Descr> Vertex_Map;