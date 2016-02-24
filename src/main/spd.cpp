#include <utility>
#include <vector>

#include "DSGraph.hpp"
#include "spd.hpp"
#include "../types/dstypes.hpp"

#include "../../../BA-shortest-paths-decomposition/include/GraphWrapper.hpp"
#include "../../../BA-shortest-paths-decomposition/include/ShortestPathDecomposer.hpp"

std::vector<IVertex> spd::build_order(const DSGraph& dsg) {
    // build up g_spd corresponding to dsg (just add verts and edges)
    GraphWrapper g_spd;
    for(std::pair<BVertex_it, BVertex_it> v_it = dsg.vertices();
            v_it.first != v_it.second; ++v_it.first) {
        g_spd.addVertex(dsg.get_IVertex(*v_it.first));
    }
    for(std::pair<BEdge_it, BEdge_it> e_it = dsg.edges();
            e_it.first != e_it.second; ++e_it.first) {
        IEdge ie = dsg.get_IEdge(*e_it.first);
        g_spd.addEdge(ie.first, ie.second);
    }

    // for an explanation of config parameters see
    // BA-shortest-path-decomposition
    ShortestPathDecomposer::Config config;
    config.test = true;
    config.debug = 0;
    config.viz = false;
    config.choose_arb_mode = 1;

    ShortestPathDecomposer spd;
    spd.init(g_spd, config);
    spd.decompose();

    return spd.getOrder();
}
