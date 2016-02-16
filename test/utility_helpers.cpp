#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/utility/helpers.hpp"
#include "../src/utility/Parser.hpp"

namespace {

TEST(helpers, verifyDS) {
    typedef std::set<IVertex> setI;

    // g3.txt
    DSGraph g3 = Parser().parse_graph_int("graphs/g3.txt");
    EXPECT_EQ(true, helpers::is_ds(g3, setI({ 0, 3, 777 })));
    EXPECT_EQ(true, helpers::is_ds(g3, g3.get_set_IVertices()));
    EXPECT_EQ(false, helpers::is_ds(g3, setI({ 3, 777 })));
    EXPECT_EQ(false, helpers::is_ds(g3, setI()));

    // g_distance
    DSGraph g_d = Parser().parse_graph_int("graphs/g_distance.txt");
    EXPECT_EQ(true, helpers::is_ds(g_d, setI({ 0, 4, 7, 11 })));
    EXPECT_EQ(true, helpers::is_ds(g_d, g_d.get_set_IVertices()));
    EXPECT_EQ(false, helpers::is_ds(g_d, setI({ 4, 11 })));
    EXPECT_EQ(false, helpers::is_ds(g_d, setI()));

    // empty.txt
    DSGraph empty = Parser().parse_graph_int("graphs/empty.txt");
    EXPECT_EQ(true, helpers::is_ds(empty, setI())); 
    EXPECT_EQ(true, helpers::is_ds(empty, empty.get_set_IVertices()));
}

}    // namespace
