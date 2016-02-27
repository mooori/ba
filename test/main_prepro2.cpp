#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/PrePro2.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/helpers.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/setops.hpp"

namespace {

typedef std::set<IVertex> setI;
typedef std::set<BVertex> setB;
typedef std::vector<setB> res_t;

TEST(PrePro2, getNIs) {
    // g14.txt
    DSGraph g14 = Parser().parse_graph_int("graphs/g14.txt");
    PrePro2 pp14(g14);
    res_t nis14 = pp14.get_n_is(setI({ 0, 16 }));

    EXPECT_EQ(3, nis14.size());
    EXPECT_EQ(helpers::i2b(g14, setI({ 1, 10, 19 })), nis14[0]);
    EXPECT_EQ(helpers::i2b(g14, setI({ 5, 7, 9, 14, 17, 18 })), nis14[1]);
    EXPECT_EQ(helpers::i2b(g14, setI({ 2, 6, 8, 15 })), nis14[2]);
    
}

}    // namespace
