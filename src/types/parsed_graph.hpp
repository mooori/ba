#ifndef PARSEDGRAPH_H
#define PARSEDGRAPH_H

#include <utility>
#include <list>

using namespace std;

/**
 * Type to hold list of vertices and adj. list of edges obtained from parsing
 * graph from file.
 *
 */

template<typename T>
struct parsed_graph {
    list<T> vertices;
    list< pair<T,T> > edges;
};

#endif
