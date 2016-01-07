#include <list>

#include <gtest/gtest.h>

#include "../src/utility/Parser.hpp"
#include "../src/utility/ListHelpers.hpp"

using namespace std;

namespace {

TEST(ParserInt, GoodGraph) {
    Parser p;
    parsed_graph<int> pg = p.parse_graph_int("g1.txt");

    // check vertices
    int arrv[6] = {0, 1, 2, 3, 4, 5};
    list<int> verts_should = ListHelpers<int>()
            .init_by_arr(arrv, sizeof(arrv)/sizeof(arrv[0]));
    EXPECT_EQ(verts_should, pg.vertices);

    // check edges
    pair<int,int> arre[5] = {pair<int, int>(0, 4), pair<int, int>(1, 3), 
            pair<int,int>(2, 3), pair<int,int>(1, 5), pair<int,int>(3,5)};
    list< pair<int,int> > edges_should = ListHelpers< pair<int,int> >()
            .init_by_arr(arre, sizeof(arre)/sizeof(arre[0]));
    EXPECT_EQ(edges_should, pg.edges);
}

TEST(ParserInt, BadVertsGraph) {
    Parser p;
    EXPECT_THROW(p.parse_graph_int("test/graph_bad_verts.txt"), runtime_error);
}

TEST(ParserInt, BadEdgesGraph) {
    Parser p;
    EXPECT_THROW(p.parse_graph_int("test/graph_bad_edges.txt"), runtime_error);
}

} // namespace
