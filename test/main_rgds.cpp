#include <list>
#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/rgds.hpp"
#include "../src/main/spd.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/helpers.hpp"

namespace {

/** To tell rgds::rgds(...) how many cores are available */
const unsigned int ncores = 4;

/** 
 * Execute rgds tests on a graph
 * @param gpath path to graph's text file
 * @param dom_num graph's domination number
 */
void test_rgds(const char* gpath, unsigned int dom_num) {
    typedef std::set<IVertex> setI;
    DSGraph dsg = Parser().parse_graph_int(gpath);
    std::vector<IVertex> spd_ord = spd::build_order(dsg);

    // test D with |D| = dom_num is found
    rgds::result_t res1 = rgds::rgds(dsg, setI(), std::list<setI>(),
            dom_num, setI(), spd_ord, ncores);
    EXPECT_EQ(true, res1.second) << gpath;
    EXPECT_TRUE(helpers::is_ds(dsg, res1.first));

    // test no D with |D| < dom_num can be found
    // however, if dom_num = 0, this is useless, so return
    if(dom_num == 0) { return; }
    rgds::result_t res2 = rgds::rgds(dsg, setI(), std::list<setI>(),
            dom_num - 1, setI(), spd_ord, ncores);
    EXPECT_EQ(false, res2.second) << gpath;
    EXPECT_EQ(setI(), res2.first) << gpath;

    return;
}

TEST(rgds, rgds) {
    test_rgds("graphs/g2.txt", 2);
    test_rgds("graphs/g3.txt", 3);
    test_rgds("graphs/g4.txt", 3);
    test_rgds("graphs/g5.txt", 3);
    test_rgds("graphs/g6.txt", 3);
    test_rgds("graphs/g7.txt", 2);
    test_rgds("graphs/g8.txt", 3);
    test_rgds("graphs/g9.txt", 2);
    test_rgds("graphs/g_distance.txt", 4);
    test_rgds("graphs/empty.txt", 0);
}

}    // namespace
