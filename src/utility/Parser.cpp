#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>

#include "Parser.hpp"
#include "../types/dstypes.hpp"

Parser::Parser() { }

DSGraph Parser::parse_graph_int(const char* fpn) {
    std::ifstream inp(fpn);
    std::string line;
    std::set<IVertex> verts;
    std::list<IEdge> edges;

    // iterate over data lines to build up set of vertices and list of edges
    while(getline(inp, line)) {
        if(line.substr(0, 1) == "%"  || line.empty()) { continue; }
        std::istringstream iss(line);
        unsigned int u, v;
        if(!(iss >> u)) {
            throw std::runtime_error("bad input: " + line);
        } else if(!(iss >> v)) {
            // could read u but not v, so it's just a vertex
            verts.insert(u);
        } else {
            verts.insert(u); verts.insert(v);
            edges.push_back(IEdge(u, v));
        }
    }

    // construct DSGraph
    DSGraph dsg;
    // remove var when DSG constant
    std::list<IVertex> vl(verts.begin(), verts.end());
    dsg.add_IVertices(vl);
    dsg.add_IEdges(edges);

    return dsg;
}
