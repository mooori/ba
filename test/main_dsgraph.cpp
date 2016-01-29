#include <list>
#include <set>
#include <utility>

#include <gtest/gtest.h>

#include "helpers_graphComp.hpp"
#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/ListHelpers.hpp"
#include "../src/utility/Parser.hpp"

namespace {

TEST(DSGraph, exceptions) {
    // cannot add same IVertex twice
    DSGraph dsg1;
    dsg1.add_IVertex(666);
    ASSERT_THROW(dsg1.add_IVertex(666), std::runtime_error);

    // ...adding vertices via list
    DSGraph dsg2;
    std::list<IVertex> vids;
    vids.push_back(666); vids.push_back(666);
    ASSERT_THROW(dsg2.add_IVertices(vids), std::runtime_error);
    
    // cannot add same IEdge twice
    DSGraph dsg3;
    dsg3.add_IVertex(0); dsg3.add_IVertex(1);
    dsg3.add_IEdge(IEdge(0, 1));
    ASSERT_THROW(dsg3.add_IEdge(IEdge(1, 0)), std::runtime_error);

    // ... adding vertices via list
    DSGraph dsg4;
    dsg4.add_IVertex(0); dsg4.add_IVertex(1);
    std::list<IEdge> es;
    es.push_back(IEdge(0, 1)); es.push_back(IEdge(1, 0));
    ASSERT_THROW(dsg4.add_IEdges(es), std::runtime_error);

    // try adding edge to IVertex that's not in G
    DSGraph dsg5;
    dsg5.add_IVertex(0); dsg5.add_IVertex(1);
    ASSERT_THROW(dsg5.add_IEdge(IEdge(0, 666)), std::out_of_range);
    ASSERT_THROW(dsg5.add_IEdge(IEdge(666, 0)), std::out_of_range);

    // try getting B/I vertices that are not in G 
    DSGraph dsg6;
    dsg6.add_IVertex(0);
    ASSERT_THROW(dsg6.get_BVertex(666), std::out_of_range);
    // BVertex is a ptr address, so 0 should never be a BVertex
    ASSERT_THROW(dsg6.get_IVertex(0), std::out_of_range);

    // getting IEdge of BEdge thats not in G
    DSGraph dsg7;
    dsg7.add_IVertex(0); dsg7.add_IVertex(1);
    ASSERT_THROW(dsg7.get_IEdge(*(dsg7.edges().first)), std::out_of_range);

}

TEST(DSGraph, emptyGraph) {
    DSGraph dsg;
    EXPECT_EQ(0, dsg.num_vertices());
    
    std::pair<BVertex_it, BVertex_it> vit = dsg.vertices();
    EXPECT_TRUE(vit.first == vit.second) << "empty G should have no vertices";

    std::pair<BEdge_it, BEdge_it> eit = dsg.edges();
    EXPECT_TRUE(eit.first == eit.second) << "empty G should have no edges";
}

TEST(DSGraph, graphConstruction) {
    // using same graph as stored in g1.txt
    DSGraph dsg;
    EXPECT_EQ(0, dsg.num_vertices());

    // add vertices
    IVertex vids_arr[6] = {0, 1, 2, 3, 4, 5};
    std::list<IVertex> vids = ListHelpers<IVertex>()
            .init_by_arr(vids_arr, sizeof(vids_arr)/sizeof(vids_arr[0]));
    dsg.add_IVertices(vids);
    EXPECT_EQ(6, dsg.num_vertices());

    // compare vertex lists
    std::pair<BVertex_it, BVertex_it> dsg_vit = dsg.vertices();
    for(std::list<IVertex>::iterator should_vit = vids.begin();
            should_vit != vids.end(); ++should_vit) {
        ASSERT_FALSE(dsg_vit.first == dsg_vit.second) <<
                "too few vertices in actual";
        EXPECT_EQ(*should_vit, dsg.get_IVertex(*dsg_vit.first));
        ++dsg_vit.first;
    }
    // reached end of vertex_should list, so also dsg_vit should be at end
    ASSERT_TRUE(dsg_vit.second == dsg_vit.first) <<
            "too many vertices in actual";

    // add edges
    IEdge es_arr[5] = { IEdge(0, 4), IEdge(1,3), IEdge(1, 5), IEdge(2, 3), 
            IEdge(3, 5) };
    std::list<IEdge> es = ListHelpers<IEdge>()
            .init_by_arr(es_arr, sizeof(es_arr)/sizeof(es_arr[0]));
    dsg.add_IEdges(es);

    // compare edge lists
    std::pair<BEdge_it, BEdge_it> dsg_eit = dsg.edges();
    for(std::list<IEdge>::iterator should_eit = es.begin();
            should_eit != es.end(); ++should_eit) {
        ASSERT_FALSE(dsg_eit.first == dsg_eit.second) <<
                "too few edges in actual";
        EXPECT_EQ(*should_eit, dsg.get_IEdge(*dsg_eit.first));
        ++dsg_eit.first;
    }
    // reached end of edges_should list, so also dsg_eit should be at end
    ASSERT_TRUE(dsg_eit.first == dsg_eit.second) << "too many edges in actual";
}

TEST(DSGraph, getAdjVertices) {
    // g1.txt
    Parser p1;
    DSGraph dsg1 = p1.parse_graph_int("graphs/g1.txt");

    // check BV neighbours of IV 4
    IVertex arr1[1] = { 0 };
    std::set<BVertex> should1 = graphComp::init_set_BVs_by_IVs(dsg1,
            arr1, sizeof(arr1)/sizeof(arr1[0]));
    std::set<BVertex> act1 = dsg1.get_adj_BVertices(dsg1.get_BVertex(4));
    EXPECT_EQ(should1.size(), act1.size()); 
    EXPECT_EQ(should1, act1);

    // check BV neighbours of IV 3
    IVertex arr2[3] = { 1, 2, 5 };
    std::set<BVertex> should2 = graphComp::init_set_BVs_by_IVs(dsg1,
            arr2, sizeof(arr2)/sizeof(arr2[0]));
    std::set<BVertex> act2 = dsg1.get_adj_BVertices(dsg1.get_BVertex(3));
    EXPECT_EQ(should2.size(), act2.size());
    EXPECT_EQ(should2, act2);

    // check IV neighbours of IV 0
    IVertex arr3[1] = { 4 };
    std::set<IVertex> should3 = graphComp::init_set_IVs_by_IVs(arr3,
            sizeof(arr3)/sizeof(arr3[0]));
    std::set<IVertex> act3 = dsg1.get_adj_IVertices(0);
    EXPECT_EQ(should3.size(), act3.size());
    EXPECT_EQ(should3, act3);

    // check IV neighbors of IV 3
    IVertex arr4[3] = { 1, 2, 5 };
    std::set<IVertex> should4 = graphComp::init_set_IVs_by_IVs(arr4,
            sizeof(arr4)/sizeof(arr4[0]));
    std::set<IVertex> act4 = dsg1.get_adj_IVertices(3);
    EXPECT_EQ(should4.size(), act4.size());
    EXPECT_EQ(should4, act4);

    // just_a_vertex.txt
    Parser p2;
    DSGraph dsg2 = p2.parse_graph_int("graphs/just_a_vertex.txt");

    // check BV neighbours of IV 0
    std::set<BVertex> act5 = dsg2.get_adj_BVertices(dsg2.get_BVertex(0));
    EXPECT_EQ(0, act5.size());
    EXPECT_EQ(std::set<BVertex>(), act5);

    // check IV neighbours of IV 0
    std::set<IVertex> act6 = dsg2.get_adj_IVertices(0);
    EXPECT_EQ(0, act6.size());
    EXPECT_EQ(std::set<IVertex>(), act6);
}

TEST(DSGraph, removeVertex) {
    Parser p;
    DSGraph dsg = p.parse_graph_int("graphs/g1.txt");

    ASSERT_EQ(6, dsg.num_vertices());

    // remove vertex and check resulting graph
    dsg.remove_IVertex(3);

    // check vertices
    ASSERT_EQ(5, dsg.num_vertices());

    IVertex arrv[5] = {0, 1, 2, 4, 5};
    std::list<IVertex> verts_should = ListHelpers<IVertex>()
            .init_by_arr(arrv, sizeof(arrv)/sizeof(arrv[0]));
    graphComp::compare_vertices(dsg, verts_should);

    // check edges
    IEdge arre[2] = {IEdge(0, 4), IEdge(1, 5)};
    std::list<IEdge> edges_should = ListHelpers<IEdge>()
            .init_by_arr(arre, sizeof(arre)/sizeof(arre[0]));
    graphComp::compare_edges(dsg, edges_should);

    // try deleting vert which isn't there and check it didn't change graph
    ASSERT_THROW(dsg.remove_IVertex(666), std::out_of_range);
    graphComp::compare_vertices(dsg, verts_should);
    graphComp::compare_edges(dsg, edges_should);

    // Boost doesn't throw anything if trying to clear a vertex which isn't
    // there, just dumps core. Would be too ugly to test.
}

TEST(DSGraph, stuff) {
    // test for functions not (indirectly) tested in graphConstruction
    // get_IEdge
    DSGraph dsg1;
    dsg1.add_IVertex(0); dsg1.add_IVertex(1); dsg1.add_IVertex(2);
    dsg1.add_IEdge(IEdge(0, 1)); dsg1.add_IEdge(IEdge(0, 2));
    std::pair<BEdge_it, BEdge_it> eit = dsg1.edges();
    ++eit.first;
    IEdge e = dsg1.get_IEdge(*eit.first);
    EXPECT_EQ(IEdge(0, 2), e);

    // get_BVertex
    DSGraph dsg2;
    dsg2.add_IVertex(0); dsg2.add_IVertex(1); dsg2.add_IVertex(2);
    std::pair<BVertex_it, BVertex_it> vit = dsg2.vertices();
    EXPECT_EQ(*vit.first, dsg2.get_BVertex(0));
    ++vit.first;
    EXPECT_EQ(*vit.first, dsg2.get_BVertex(1));
    ++vit.first;
    EXPECT_EQ(*vit.first, dsg2.get_BVertex(2));
}

} // namespace
