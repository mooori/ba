#ifndef PARSER_H
#define PARSER_H

#include "../main/DSGraph.hpp"

/**
 * Parser to read Graphs from text files.
 */

class Parser {
public:

    /**
     * Constructor, just creating "empty object".
     * File format:
     *     - no blanks, no empty lines at top||bottom
     *     - comments starting with "//" allowed, only at beginning of line
     *     - 1st part - vertices: one vertex name per line
     *     - empty line
     *     - 2nd part - edges: one edge per line, vertex names sep by whitespace
     *
     * The following types of vertex names are accepted:
     *     - int
     */
    Parser();

    /**
     * Parse graph with vertex names of type int.
     * @param fpn file name (incl. full path to file)
     * @return corresponding DSGraph
     * @throws std::runtime_error if input graph without vertices has edges
     */
    DSGraph parse_graph_int(const char* fpn);
};

#endif
