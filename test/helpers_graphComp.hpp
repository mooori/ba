#ifndef HELPERS_GRAPHCOMP_H
#define HELPERS_GRAPHCOMP_H

#include <list>
#include <set>

#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"

namespace graphComp {

/** * Compare vertices in DSGraph to vertices provided in list.  */
void compare_vertices(DSGraph& dsg, std::list<IVertex>& should);

/** * Compare edges in DSGraph to edges provided in list.  */
void compare_edges(DSGraph& dsg, std::list<IEdge>& should);

/** * Init set of BVertices given array of IVertices */
std::set<BVertex> init_set_BVs_by_IVs(DSGraph& dsg, IVertex vids[],
        size_t vids_size);

/** Init set of IVertices given array of IVertices */
std::set<IVertex> init_set_IVs_by_IVs(IVertex vids[], size_t vids_size);

} // namespace

#endif
