#include <iostream>
#include <set>

#include "helpers.hpp"
#include "../main/DSGraph.hpp"
#include "../main/FuncIter.hpp"
#include "../types/dstypes.hpp"

void helpers::print_vs(const DSGraph& dsg) {
    std::set<IVertex> vs = dsg.get_set_IVertices();
    std::cout << "IVertices = { ";
    for(std::set<IVertex>::const_iterator vs_it = vs.begin();
            vs_it != vs.end(); ) {
        std::cout << *vs_it;
        if(++vs_it != vs.end()) { std::cout << ", "; }
    }
    std::cout << " }\n";
    return;
}

void helpers::print_es(const DSGraph& dsg) {
    std::cout << "IEdges = { ";
    for(std::pair<BEdge_it, BEdge_it> it = dsg.edges(); it.first != it.second; ) {
        IEdge ie = dsg.get_IEdge(*it.first);
        std::cout << ie.first << "-" << ie.second;
        if(++it.first != it.second) { std::cout << ", "; }
    }
    std::cout << " }\n";
    return;
}

void helpers::print(const DSGraph& dsg) {
    helpers::print_vs(dsg);
    helpers::print_es(dsg);
}

void helpers::print(const FuncIter& f, const unsigned int ndigs) {
    std::cout << "FuncIter: ";
    for(unsigned int i = 0; i < ndigs; ++i) {
        std::cout << f.get_digit_val(i);
        if(i < ndigs - 1) { std::cout << " - "; }
    }
    std::cout << "\n";
    return;
}

void helpers::print(const std::string name, const std::set<IVertex> s) {
    std::cout << name << "= { ";
    for(std::set<IVertex>::const_iterator it = s.begin(); it != s.end(); ) {
        std::cout << *it;
        if(++it != s.end()) { std::cout << ", "; }
    }
    std::cout << " }\n";
    return;
}

void helpers::print(const std::string name,
        const std::list< std::set<IVertex> > l) {
    std::cout << name << " = {\n";
    int i = 0;
    for(std::list< std::set<IVertex> >::const_iterator it = l.begin();
            it != l.end(); ++it) {
        helpers::print(name + "[" + std::to_string(i++) + "]", *it);
    }
    std::cout << "}\n";
}
