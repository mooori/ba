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
    parsed_graph<int> pg = Parser().parse_graph_int("test/g_distance.txt");
    DSGraph g(pg.vertices.size());
    for(std::list< std::pair<int, int> >::iterator it = pg.edges.begin();
            it != pg.edges.end(); ++it) {
        DSEdge e = *it;
        boost::add_edge(e.first, e.second, g);
    }

    distance_info di = Distance<DSGraph>().get_info(g);

    EXPECT_EQ(3, di.radius);
    EXPECT_EQ(6, di.diameter);
}

} // namespace
