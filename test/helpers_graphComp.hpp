#ifndef HELPERS_GRAPHCOMP_H
#define HELPERS_GRAPHCOMP_H

#include <list>

#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"

namespace graphComp {

void compare_vertices(DSGraph& dsg, std::list<IVertex>& should);

void compare_edges(DSGraph& dsg, std::list<IEdge>& should);

} // namespace

#endif
