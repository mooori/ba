#include <list>
#include <utility>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/ListHelpers.hpp"

namespace {

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
