#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/utility/rand_g.hpp"

namespace {

TEST(randG, randG) {
    // n=0 returns empty graph
    DSGraph dsg1 = rand_g::create(0);
    EXPECT_EQ(0, dsg1.num_vertices());
    EXPECT_TRUE(dsg1.edges().first == dsg1.edges().second);

    // n != 0
    DSGraph dsg2 = rand_g::create(36);
    EXPECT_EQ(36, dsg2.num_vertices());
}

}    // namespace
