#include <list>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/spd.hpp"
#include "../src/utility/Parser.hpp"

namespace {

/**
 * Get spd_order for graph and verify that all vertices are contained in it
 */
void test_order(const char* gpath) {
    DSGraph dsg = Parser().parse_graph_int(gpath);
    std::vector<IVertex> ord = spd::build_order(dsg);

    EXPECT_EQ(dsg.num_vertices(), ord.size()) << gpath;

    // verify all vertices in ord by adding ord's elements to set and 
    // comparing to dsg.get_set_IVertices()
    // Not efficient, but for small test graphs Ok.
    // Advantage: On unequal both sets printed -> easy to see diffs
    EXPECT_EQ(dsg.get_set_IVertices(),
            std::set<IVertex>(ord.begin(), ord.end())) << gpath;

    return;
}

TEST(spd, buildOrder) {

    std::list<const char*> gs{
        "graphs/g1.txt",
        "graphs/g2.txt",
        "graphs/g3.txt",
        "graphs/g4.txt",
        "graphs/g5.txt",
        "graphs/g6.txt",
        "graphs/g7.txt",
        "graphs/g8.txt",
        "graphs/g9.txt",
        "graphs/g_distance.txt",
        "graphs/empty.txt",
        "graphs/just_a_vertex"
    };
    
    for(std::list<const char*>::iterator gs_it = gs.begin();
            gs_it != gs.end(); ++gs_it) {
        test_order(*gs_it);
    }

}

}
