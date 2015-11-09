#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#include "Parser.hpp"

Parser::Parser() { }

void Parser::hey() {
    parsed_graph<int> pg;
    int i = 13;
    pg.vertices.push_back(i);
    cout << "Hey there, world!\nYour parser is speaking.\n" 
            << pg.vertices.front() << "\n";
    return;
}

parsed_graph<int> Parser::parse_graph_int(string fpn) {
    ifstream inp(fpn);
    string line;
    parsed_graph<int> pg;

    // read and store vertices till blank line met
    while(getline(inp, line) && !line.empty()) {
        pg.vertices.push_back(stoi(line));
    }

    // graph without vertices may not have edges
    if (pg.vertices.empty()) { return pg; }

    // read and store edges
    while(getline(inp, line)) {
        istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) { throw runtime_error("bad input (edge)"); }
        pg.edges.push_back(pair<int,int>(u,v));
    }

    return pg;   
}
