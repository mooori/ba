#include <list>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <gtest/gtest.h>

#include "../src/types/dstypes.hpp"
#include "../src/types/distance_info.hpp"
#include "../src/main/Distance.hpp"
#include "../src/utility/Parser.hpp"

namespace {

TEST(Distance, getInfo) {
    // construct graph
    DSGraph dsg = Parser().parse_graph_int("g_distance.txt");
    
    distance_info di = Distance().get_info(dsg);

    EXPECT_EQ(3, di.radius);
    EXPECT_EQ(6, di.diameter);
}

} // namespace
