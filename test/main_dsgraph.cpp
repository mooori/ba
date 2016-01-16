#include <list>
#include <utility>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/ListHelpers.hpp"

namespace {

TEST(DSGraph, Exceptions) {
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

TEST(DSGraph, EmptyGraph) {
    DSGraph dsg;
    EXPECT_EQ(0, dsg.num_vertices());
    
    std::pair<BVertex_it, BVertex_it> vit = dsg.vertices();
    EXPECT_TRUE(vit.first == vit.second) << "empty G should have no vertices";

    std::pair<BEdge_it, BEdge_it> eit = dsg.edges();
    EXPECT_TRUE(eit.first == eit.second) << "empty G should have no edges";
}

TEST(DSGraph, GraphConstruction) {
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

} // namespace
