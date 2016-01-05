#ifndef PARSEDGRAPH_H
#define PARSEDGRAPH_H

#include <utility>
#include <list>

/**
 * Type to hold list of vertices and adj. list of edges obtained from parsing
 * graph from file.
 *
 */

template<typename T>
struct parsed_graph {
    std::list<T> vertices;
    std::list< std::pair<T,T> > edges;
};

#endif
