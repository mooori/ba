#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/utility/bfds.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/helpers.hpp"

namespace {

/**
 * Execute bfds tests on a graph
 * @param gpath path to graph's text file
 * @param dom_num graph's domination number
 */
void test_bfds(const char* gpath, unsigned int dom_num) {
    DSGraph dsg = Parser().parse_graph_int(gpath);
    std::set<IVertex> res = bfds::get_min_ds(dsg);

    EXPECT_EQ(dom_num, res.size());
    EXPECT_TRUE(helpers::is_ds(dsg, res));
    return;
}

TEST(bfds, bfds) {
    test_bfds("graphs/g2.txt", 2);
    test_bfds("graphs/g3.txt", 3);
    test_bfds("graphs/g4.txt", 3);
    test_bfds("graphs/g5.txt", 3);
    test_bfds("graphs/g6.txt", 3);
    test_bfds("graphs/g7.txt", 2);
    test_bfds("graphs/g8.txt", 3);
    test_bfds("graphs/g9.txt", 2);
    test_bfds("graphs/g10.txt", 5);
    test_bfds("graphs/g_distance.txt", 4);
    test_bfds("graphs/empty.txt", 0);
}
    

}    // namespace
