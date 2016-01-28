#include <list>
#include <utility>

#include <gtest/gtest.h>

#include "helpers_graphComp.hpp"
#include "../src/main/DSGraph.hpp"
#include "../src/types/dstypes.hpp"

void graphComp::compare_vertices(DSGraph& dsg, std::list<IVertex>& should) {
    std::pair<BVertex_it, BVertex_it> act_it = dsg.vertices();

    for(std::list<IVertex>::iterator should_it = should.begin();
            should_it != should.end(); ++should_it) {
        ASSERT_FALSE(act_it.first == act_it.second) <<
                "too few vertices in actual";
        EXPECT_EQ(*should_it, dsg.get_IVertex(*act_it.first));
        ++act_it.first;
    }

    ASSERT_TRUE(act_it.first == act_it.second) <<
            "too many vertices in actual";
    return;
}

void graphComp::compare_edges(DSGraph& dsg, std::list<IEdge>& should) {
    std::pair<BEdge_it, BEdge_it> act_it = dsg.edges();

    for(std::list<IEdge>::iterator should_it = should.begin();
            should_it != should.end(); ++should_it) {
        ASSERT_FALSE(act_it.first == act_it.second) <<
                "too few edges in actual";
        EXPECT_EQ(*should_it, dsg.get_IEdge(*act_it.first));
        ++act_it.first;
    }

    ASSERT_TRUE(act_it.first == act_it.second) << "too many edges in actual";
    return;
}
