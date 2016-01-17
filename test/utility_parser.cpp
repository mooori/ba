#include <list>

#include <gtest/gtest.h>

#include "../src/utility/Parser.hpp"
#include "../src/utility/ListHelpers.hpp"

namespace {

TEST(ParserInt, goodGraph) {
    Parser p;
    DSGraph dsg = p.parse_graph_int("graphs/g1.txt");

    // check vertices
    EXPECT_EQ(6, dsg.num_vertices());

    IVertex arrv[6] = {0, 1, 2, 3, 4, 5};
    std::list<IVertex> verts_should = ListHelpers<IVertex>()
            .init_by_arr(arrv, sizeof(arrv)/sizeof(arrv[0]));

    std::pair<BVertex_it, BVertex_it> dsg_vit = dsg.vertices();
    for(std::list<IVertex>::iterator should_vit = verts_should.begin();
            should_vit != verts_should.end(); ++should_vit) {
        ASSERT_FALSE(dsg_vit.first == dsg_vit.second) <<
                "too few vertices in actual";
        EXPECT_EQ(*should_vit, dsg.get_IVertex(*dsg_vit.first));
        ++dsg_vit.first;
    }
    ASSERT_TRUE(dsg_vit.first == dsg_vit.second) <<
            "too many vertices in actual";

    // check edges
    IEdge arre[5] = {IEdge(0, 4), IEdge(1, 3), IEdge(2, 3), IEdge(1, 5),
            IEdge(3,5)};
    std::list<IEdge> edges_should = ListHelpers<IEdge>()
            .init_by_arr(arre, sizeof(arre)/sizeof(arre[0]));

    std::pair<BEdge_it, BEdge_it> dsg_eit = dsg.edges();
    for(std::list<IEdge>::iterator should_eit = edges_should.begin();
            should_eit != edges_should.end(); ++should_eit) {
        ASSERT_FALSE(dsg_eit.first == dsg_eit.second) <<
                "too few edges in actual";
        EXPECT_EQ(*should_eit, dsg.get_IEdge(*dsg_eit.first));
        ++dsg_eit.first;
    }
    ASSERT_TRUE(dsg_eit.first == dsg_eit.second) << "too many edges in actual";
}

TEST(ParserInt, exceptions) {
    // bad vertices
    Parser p1;
    EXPECT_THROW(p1.parse_graph_int("graphs/bad_verts.txt"),
            std::runtime_error);

    // bad edges
    Parser p2;
    EXPECT_THROW(p2.parse_graph_int("graphs/bad_edges.txt"),
            std::runtime_error);

    // no verts but edges
    Parser p3;
    EXPECT_THROW(p3.parse_graph_int("graphs/no_verts_but_edges.txt"),
            std::runtime_error);
}

} // namespace
