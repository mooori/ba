#ifndef PARSER_H
#define PARSER_H

#include "../main/DSGraph.hpp"

/**
 * Parser to read Graphs from text files.
 *
 * File format as used by Konect (http://konect.uni-koblenz.de/)
 *     - comments starting with "%" allowed, only at beginning of line
 *     - one edge per line: src and target verts separated by whitespace/tab
 *
 * Issue with this format: How to add vertex that doesn't have edges?
 * Parser allows for lines with single vertex id (unsigned int); just the
 * vertex without any edges is added.
 * 
 * Some graphs contain more than two columns per line. First two columns are
 * always src and target vertices. Following columns may contain info like
 * edge weights, time stamps, etc
 * (see http://konect.uni-koblenz.de/downloads/konect-handbook.pdf).
 * Any data other than src and target vertex is ignored.
 *
 * The following types of vertex names are accepted:
 *     - int
 */

class Parser {
public:

    /**
     * Constructor, just creating "empty object".
     */

    Parser();

    /**
     * Parse graph with vertex names of type int.
     * @param fpn file name (incl. path relativ to calling location)
     * @return corresponding DSGraph
     * @throws std::runtime_error if input graph without vertices has edges
     */
    DSGraph parse_graph_int(const char* fpn);
};

#endif
