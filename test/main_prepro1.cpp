#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/PrePro1.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/helpers.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/setops.hpp"

namespace {

typedef std::set<IVertex> setI;
typedef std::set<BVertex> setB;
typedef std::vector<setB> res_t;

TEST(PrePro1, apply) {
    // g11.txt
    DSGraph g11 = Parser().parse_graph_int("graphs/g11.txt");
    PrePro1 pp11(g11);
    pp11.apply();

    // check pp attributes
    EXPECT_EQ(setI(), pp11.pre_H);
    EXPECT_EQ(setI({ 0, 1, 2, 7 }), pp11.pre_D);
    EXPECT_EQ(setI(), pp11.no_effect);
}

/** Test if union of nis cover entire neighbourhood */
void nis_cover_n(std::vector< std::set<BVertex> > nis, std::set<BVertex> n) {
    std::list< std::set<BVertex> > nis_list(nis.begin(), nis.end());
    EXPECT_EQ(setops::big_union_new(nis_list), n);
    return;
}

TEST(PrePro1, getNIs) {
    // g11.txt
    DSGraph g11 = Parser().parse_graph_int("graphs/g11.txt");
    PrePro1 pp11(g11);
    res_t nis11 = pp11.get_n_is(g11.get_BVertex(0));

    EXPECT_EQ(3, nis11.size());
    EXPECT_EQ(helpers::i2b(g11, setI({ 1, 2, 7 })), nis11[0]);
    EXPECT_EQ(helpers::i2b(g11, setI({ 3, 4, 6, 8 })), nis11[1]);
    EXPECT_EQ(helpers::i2b(g11, setI({ 5, 9 })), nis11[2]);
    nis_cover_n(nis11, g11.get_adj_BVertices(g11.get_BVertex(0)));

    // g12.txt
    DSGraph g12 = Parser().parse_graph_int("graphs/g12.txt");
    PrePro1 pp12(g12);
    res_t nis12 = pp12.get_n_is(g12.get_BVertex(0));

    EXPECT_EQ(3, nis12.size());
    EXPECT_EQ(helpers::i2b(g12, setI({ })), nis12[0]);
    EXPECT_EQ(helpers::i2b(g12, setI({ })), nis12[1]);
    EXPECT_EQ(g12.get_adj_BVertices(g12.get_BVertex(0)), nis12[2]);
    nis_cover_n(nis12, g12.get_adj_BVertices(g12.get_BVertex(0)));

    // just_a_vertex.txt
    DSGraph jav = Parser().parse_graph_int("graphs/just_a_vertex.txt");
    PrePro1 pp_jav(jav);
    res_t nis_jav = pp_jav.get_n_is(jav.get_BVertex(0));

    EXPECT_EQ(3, nis_jav.size());
    EXPECT_TRUE(nis_jav[0].empty());
    EXPECT_TRUE(nis_jav[1].empty());
    EXPECT_TRUE(nis_jav[2].empty());
    nis_cover_n(nis_jav, jav.get_adj_BVertices(jav.get_BVertex(0)));
}

}    // namespace
