#ifndef HELPERS_H
#define HELPERS_H

#include <list>
#include <set>
#include <string>

#include "../main/DSGraph.hpp"
#include "../main/FuncIter.hpp"

namespace helpers {

    /*
     * Verify if set of IVertices d is dom set for graph dsg
     * @throws @see DSGraph::get_adj_IVertices
     */
    bool is_ds(const DSGraph& dsg, const std::set<IVertex>& d);

    /** Print DSGraph's IVertices */
    void print_vs(const DSGraph& dsg);

    /** Print DSGraph's IEdges. For readability 'u-v' instead of '{ u, v }' */
    void print_es(const DSGraph& dsg);

    /** Print DSGraph's IVertices and IEdges */
    void print(const DSGraph& dsg);

    /**
     * Print FuncIter's current 'number'; digits separated by ' - '
     * @param ndigs size of FuncIter's number
     */
    void print(const FuncIter& f, const unsigned int ndigs);

    /** Print set of IVertices */
    void print(const std::string name, const std::set<IVertex> s);

    /** Print list of sets of IVertices */
    void print(const std::string name, const std::list< std::set<IVertex> > l); 

}    // namespace

#endif
