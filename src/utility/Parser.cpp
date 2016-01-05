#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>
#include <boost/lexical_cast.hpp>

#include "Parser.hpp"

Parser::Parser() { }

void Parser::hey() {
    parsed_graph<int> pg;
    int i = 13;
    pg.vertices.push_back(i);
    std::cout << "Hey there, world!\nYour parser is speaking.\n" 
            << pg.vertices.front() << "\n";
    return;
}

parsed_graph<int> Parser::parse_graph_int(const char* fpn) {
    std::ifstream inp(fpn);
    std::string line;
    parsed_graph<int> pg;

    // read and store vertices till blank line met
    while(getline(inp, line) && !line.empty()) {
        int vname;
        try { 
            vname = boost::lexical_cast<int>(line);
        }
        catch(const boost::bad_lexical_cast& e) {
            throw std::runtime_error("bad input (vertex): \"" + line + "\"");
        }
        pg.vertices.push_back(vname);
    }

    // graph without vertices may not have edges
    if (pg.vertices.empty()) { return pg; }

    // read and store edges
    while(getline(inp, line)) {
        std::istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) {
            throw std::runtime_error("bad input (edge): \"" + line + "\"");
        }
        pg.edges.push_back(std::pair<int,int>(u,v));
    }

    return pg;   
}
