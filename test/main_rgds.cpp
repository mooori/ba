#include <list>
#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/PrePro1.hpp"
#include "../src/main/PrePro2.hpp"
#include "../src/main/rgds.hpp"
#include "../src/main/spd.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/helpers.hpp"
#include "../src/utility/setops.hpp"

namespace {

/** To tell rgds::rgds(...) how many cores are available */
const unsigned int ncores = 4;

typedef std::set<IVertex> setI;

/** 
 * Execute rgds tests on a graph. Only one of pp1 and pp2 may be true
 * @param gpath path to graph's text file
 * @param dom_num graph's domination number
 * @param pp1 if true, Preprocessing rule 1 is applied
 * @param pp2 if true, Preprocessing rule 2 is applied
 */
void test_rgds(const char* gpath, unsigned int dom_num, bool pp1, bool pp2) {
    DSGraph dsg = Parser().parse_graph_int(gpath);
    DSGraph dsg_orig(dsg);    // as PreProc changes dsg
    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    std::set<IVertex> pre_H, pre_D;

    if(pp1 && pp2) { throw std::runtime_error("cannot combine pp1 and pp2"); }

    // preprocessing
    if(pp1) {
        PrePro1 pp1(dsg);
        pp1.run();
        pre_H = pp1.pre_H;
        pre_D = pp1.pre_D;
    }
    else if(pp2) {
        PrePro2 pp2(dsg);
        pp2.run();
        pre_H = pp2.pre_H;
        pre_D = pp2.pre_D;
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
    test_rgds("graphs/g2.txt", 2, false, false);
    test_rgds("graphs/g3.txt", 3, false, false);
    test_rgds("graphs/g4.txt", 3, false, false);
    test_rgds("graphs/g5.txt", 3, false, false);
    test_rgds("graphs/g6.txt", 3, false, false);
    test_rgds("graphs/g7.txt", 2, false, false);
    test_rgds("graphs/g8.txt", 3, false, false);
    test_rgds("graphs/g9.txt", 2, false, false);
    test_rgds("graphs/g11.txt", 4, false, false);
    test_rgds("graphs/g15.txt", 2, false, false);
    test_rgds("graphs/g_distance.txt", 4, false, false);
    test_rgds("graphs/just_a_vertex.txt", 1, false, false);
    test_rgds("graphs/empty.txt", 0, false, false);
}

TEST(rgds, rgdsPP1) {
    test_rgds("graphs/g2.txt", 2, true, false);
    test_rgds("graphs/g3.txt", 3, true, false);
    test_rgds("graphs/g4.txt", 3, true, false);
    test_rgds("graphs/g5.txt", 3, true, false);
    test_rgds("graphs/g6.txt", 3, true, false);
    test_rgds("graphs/g7.txt", 2, true, false);
    test_rgds("graphs/g8.txt", 3, true, false);
    test_rgds("graphs/g9.txt", 2, true, false);
    test_rgds("graphs/g11.txt", 4, true, false);
    test_rgds("graphs/g15.txt", 2, true, false);
    test_rgds("graphs/g_distance.txt", 4, true, false);
    test_rgds("graphs/empty.txt", 0, true, false);
}

TEST(rgds, rgdsPP2) {
    test_rgds("graphs/g2.txt", 2, false, true);
    test_rgds("graphs/g3.txt", 3, false, true);
    test_rgds("graphs/g4.txt", 3, false, true);
    test_rgds("graphs/g5.txt", 3, false, true);
    test_rgds("graphs/g6.txt", 3, false, true);
    test_rgds("graphs/g7.txt", 2, false, true);
    test_rgds("graphs/g8.txt", 3, false, true);
    test_rgds("graphs/g9.txt", 2, false, true);
    test_rgds("graphs/g11.txt", 4, false, true);
    test_rgds("graphs/g15.txt", 2, false, true);
    test_rgds("graphs/g_distance.txt", 4, false, true);
    test_rgds("graphs/empty.txt", 0, false, true);
    test_rgds("graphs/g14.txt", 5, false, true);
}

TEST(rgds, preHpreD) {
    // g13.txt with pre_D and pre_H
    DSGraph dsg1 = Parser().parse_graph_int("graphs/g13.txt");
    EXPECT_EQ(17, dsg1.num_vertices());
    setI pre_H1{ 6, 8, 15, 16, 19, 24, 31, 32, 36, 44, 45, 59 };
    setI pre_D1{ 1, 7, 10, 17, 18, 22, 30, 46, 49 };

    // Given pre_H, { 16, 36 } can dominate dsg1
    setI exp1(pre_D1);
    exp1.insert(16); exp1.insert(36);
    std::vector<IVertex> spd_ord1 = spd::build_order(dsg1);
    rgds::result_t res1 = rgds::rgds(dsg1, pre_H1, std::list<setI>(), 2,
            pre_D1, spd_ord1, 1);
    EXPECT_EQ(true, res1.second);
    EXPECT_EQ(exp1, res1.first);


    DSGraph dsg2 = Parser().parse_graph_int("graphs/g1.txt");
    EXPECT_EQ(6, dsg2.num_vertices());
    std::vector<IVertex> spd_ord2 = spd::build_order(dsg2);
    rgds::result_t res2 = rgds::rgds(dsg2, setI(), std::list<setI>(), 2,
            setI({ 666 }), spd_ord2, 1);
    EXPECT_EQ(true, res2.second);
    EXPECT_EQ(setI({ 0, 3, 666 }), res2.first);

    // intersect with VG, so that helpers::is_ds can be used
    setI VG2 = dsg2.get_set_IVertices();
    EXPECT_TRUE(helpers::is_ds(dsg2, setops::inters_new(res2.first, VG2)));
}

}    // namespace
