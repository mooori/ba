#include <list>
#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/PrePro1.hpp"
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
 * @param pp1 if true, Preprocessing rule 1 is applied
 */
void test_rgds(const char* gpath, unsigned int dom_num, bool pp1) {
    typedef std::set<IVertex> setI;
    DSGraph dsg = Parser().parse_graph_int(gpath);
    DSGraph dsg_orig(dsg);    // as PreProc changes dsg
    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    std::set<IVertex> pre_H, pre_D;

    // preprocessing
    if(pp1) {
        PrePro1 pp1(dsg);
        pp1.run();
        pre_H = pp1.pre_H;
        pre_D = pp1.pre_D;
    }

    // test D with |D| = dom_num is found
    EXPECT_LE(pre_D.size(), dom_num);
    rgds::result_t res1 = rgds::rgds(dsg, pre_H, std::list<setI>(),
            dom_num - pre_D.size(), pre_D, spd_ord, ncores);
    EXPECT_EQ(true, res1.second) << gpath;
    EXPECT_TRUE(helpers::is_ds(dsg_orig, res1.first));

    // test no D with |D| < dom_num can be found
    // however, if dom_num = 0, this is useless, so return
    if(dom_num == 0) { return; }
    if(pre_D.size() > dom_num - 1) { return; }    // means D with no |D| < k - 1
    rgds::result_t res2 = rgds::rgds(dsg, pre_H, std::list<setI>(),
            dom_num - pre_D.size() - 1, pre_D, spd_ord, ncores);
    EXPECT_EQ(false, res2.second) << gpath;
    EXPECT_EQ(setI(), res2.first) << gpath;

    return;
}

TEST(rgds, rgdsNoPP) {
    test_rgds("graphs/g2.txt", 2, false);
    test_rgds("graphs/g3.txt", 3, false);
    test_rgds("graphs/g4.txt", 3, false);
    test_rgds("graphs/g5.txt", 3, false);
    test_rgds("graphs/g6.txt", 3, false);
    test_rgds("graphs/g7.txt", 2, false);
    test_rgds("graphs/g8.txt", 3, false);
    test_rgds("graphs/g9.txt", 2, false);
    test_rgds("graphs/g11.txt", 4, false);
    test_rgds("graphs/g_distance.txt", 4, false);
    test_rgds("graphs/empty.txt", 0, false);
}

TEST(rgds, rgdsPP1) {
    test_rgds("graphs/g2.txt", 2, true);
    test_rgds("graphs/g3.txt", 3, true);
    test_rgds("graphs/g4.txt", 3, true);
    test_rgds("graphs/g5.txt", 3, true);
    test_rgds("graphs/g6.txt", 3, true);
    test_rgds("graphs/g7.txt", 2, true);
    test_rgds("graphs/g8.txt", 3, true);
    test_rgds("graphs/g9.txt", 2, true);
    test_rgds("graphs/g11.txt", 4, true);
    test_rgds("graphs/g_distance.txt", 4, true);
    test_rgds("graphs/empty.txt", 0, true);
}
}    // namespace
