#include <vector>
#include <list>

#include<boost/config.hpp>
#include<boost/graph/adjacency_list.hpp>

#include "gtest/gtest.h"

#include "../src/main/ConnComps.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/ListHelpers.hpp"

using namespace std;
using namespace boost;

namespace {

/**
 * Test if g's vertex original ids equal their expected values (should_vs)
 * @param should_vs vector of expected original ids
 * @param g
 */
void compare_v_original_ids(DSVertex should_vs[], DSGraph& comp) {
    DSProp_Acc_original_id comp_orig_ids = get(vertex_original_id, comp);
    int i = 0;
    for(pair<DS_vertex_it_t, DS_vertex_it_t> vit = vertices(comp);
            vit.first != vit.second; ++vit.first) {
        EXPECT_EQ(should_vs[i], get(comp_orig_ids, *vit.first));
        ++i;
    }
    return;
}

/**
 * Test if a vertex is contained in an adjacency list. This function is not
 *      efficient, but for small test graphs that's OK.
 * @param v vertex to search
 * @param al_it pair of adjacency list iterators
 * @return 1 if v in adjaceny list, else 0
 */
int vert_in_adj_list(DSVertex v, pair<DS_adjacency_it, DS_adjacency_it> al_it) {
    for(; al_it.first != al_it.second; ++al_it.first) {
        if(v == *al_it.first) { return 1; }
    }
    return 0;
}

/**
 * Check if a component contains all the edges it is supposed to. For each v
 *      in V(comp), check if it has same degree as v_oid in g. Then for each
 *      u in al_comp(v), check u_oid is in al_g(v_oid).
 */
void check_edges(DSGraph& g, DSGraph& comp) {
    DSProp_Acc_original_id comp_orig_ids = get(vertex_original_id, comp);

    for(pair<DS_vertex_it_t, DS_vertex_it_t> vit_comp = vertices(comp);
            vit_comp.first != vit_comp.second; ++vit_comp.first) {
        DSVertex v = *vit_comp.first;
        DSVertex v_oid = get(comp_orig_ids, v);
        EXPECT_EQ(out_degree(v_oid, g), out_degree(v, comp));
        pair<DS_adjacency_it, DS_adjacency_it> al_comp_v, al_g_v_oid;
        al_comp_v = adjacent_vertices(v, comp);
        al_g_v_oid = adjacent_vertices(v_oid, g);

        for (; al_comp_v.first != al_comp_v.second; ++al_comp_v.first) {
            DSVertex u = *al_comp_v.first;
            DSVertex u_oid = get(comp_orig_ids, u);
            EXPECT_TRUE(vert_in_adj_list(u_oid, al_g_v_oid));
        }
    }
}


TEST(ConnComps, intGraph2Comps) {
    // put g1.txt into BGL graph
    Parser p;
    parsed_graph<int> pg = p.parse_graph_int("g1.txt");
    // typedef pair<int, int> Edge;
    DSGraph g(pg.vertices.size());
    for(std::list< pair<int, int> >::iterator it = pg.edges.begin();
            it != pg.edges.end(); ++it) {
        pair<int, int> e = *it;
        add_edge(e.first, e.second, g);
    }

    vector<DSGraph*> comps = ConnComps<DSGraph>().find(g);

    // correct number of connected components found?
    EXPECT_EQ(2, comps.size());

    // correct number of vertices||edges per conn comp?
    EXPECT_EQ(2, num_vertices(*comps[0]));
    EXPECT_EQ(1, num_edges(*comps[0]));
    EXPECT_EQ(4, num_vertices(*comps[1]));
    EXPECT_EQ(4, num_edges(*comps[1]));

    // orig_id's are correct?
    DSVertex should_vs_0[2] = { 0, 4 };
    DSVertex should_vs_1[4] = { 1, 2, 3, 5 };
    compare_v_original_ids(should_vs_0, *comps[0]);
    compare_v_original_ids(should_vs_1, *comps[1]);

    // edges are correct?
    check_edges(g, *comps[0]);
    check_edges(g, *comps[1]);   
}

} // namespace
