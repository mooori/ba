#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>
#include <boost/lexical_cast.hpp>

#include "Parser.hpp"
#include "../types/dstypes.hpp"

Parser::Parser() { }

DSGraph Parser::parse_graph_int(const char* fpn) {
    std::ifstream inp(fpn);
    std::string line;
    std::list<IVertex> verts;
    std::list<IEdge> edges;

    // read and store vertices till blank line met
    while(getline(inp, line) && !line.empty()) {
        if(line.substr(0, 2) == "//") { continue; } // skip comments
        int vname;
        try { 
            vname = boost::lexical_cast<int>(line);
        }
        catch(const boost::bad_lexical_cast& e) {
            throw std::runtime_error("bad input (vertex): \"" + line + "\"");
        }
        verts.push_back(vname);
    }

    // read and store edges
    while(getline(inp, line)) {
        if(line.substr(0, 2) == "//") { continue; }
        std::istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) {
            throw std::runtime_error("bad input (edge): \"" + line + "\"");
        }
        edges.push_back(IEdge(u,v));
    }

    // test for bad input
    // graph without vertices may not have edges
    if (verts.empty() && !edges.empty()) {
        throw std::runtime_error("graph without vertices may not have edges");
    }

    // construct DSGraph
    DSGraph dsg;
    dsg.add_IVertices(verts);
    dsg.add_IEdges(edges);

    return dsg;
}
