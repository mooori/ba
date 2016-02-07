#include <list>

#include <gtest/gtest.h>

#include "../src/types/dstypes.hpp"
#include "../src/types/distance_info.hpp"
#include "../src/main/Distance.hpp"
#include "../src/utility/Parser.hpp"

namespace {

TEST(Distance, getInfo) {
    // graph with non-trivial rad & diam
    DSGraph dsg1 = Parser().parse_graph_int("graphs/g_distance.txt");
    distance_info di1 = Distance().get_info(dsg1);
    EXPECT_EQ(3, di1.radius);
    EXPECT_EQ(6, di1.diameter);

    // edge case graphs
    // empty graph
    DSGraph dsg2 = Parser().parse_graph_int("graphs/empty.txt");
    distance_info di2 = Distance().get_info(dsg2);
    EXPECT_EQ(0, di2.radius);
    EXPECT_EQ(0, di2.diameter);

    // graph consisting of just a single vertex
    DSGraph dsg3 = Parser().parse_graph_int("graphs/just_a_vertex.txt");
    distance_info di3 = Distance().get_info(dsg3);
    EXPECT_EQ(0, di3.radius);
    EXPECT_EQ(0, di3.diameter);

}

} // namespace
