#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "../types/parsed_graph.hpp"

using namespace std;

/**
 * Parser to read Graphs from text files.
 */

class Parser {
public:

    /**
     * Constructor, just creating "empty object".
     * File format:
     *     - no blanks, no empty lines at top||bottom
     *     - 1st part - vertices: one vertex name per line
     *     - empty line
     *     - 2nd part - edges: one edge per line, vertex names sep by whitespace
     *
     * The following types of vertex names are accepted:
     *     - int
     */
    Parser();

    void hey();

    /**
     * Parse graph with vertex names of type int.
     * @param fpn file name (incl. full path to file)
     * @return corresponding parsed_graph (i.e. lists of vertices and edges)
     */
    parsed_graph<int> parse_graph_int(const char* fpn);
};

#endif
