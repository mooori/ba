#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/rgds.hpp"
#include "../src/main/spd.hpp"
#include "../src/utility/add_ds.hpp"
#include "../src/utility/helpers.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/utility/rand_g_ex_Kt.hpp"

namespace {

typedef std::set<IVertex> setI;

TEST(addDS, random) {
    unsigned int n = 15;
    unsigned int t = 5;
    unsigned int ncores = 1;
    DSGraph dsg_orig = rand_g_ex_Kt::create(n, t).first;

    for(unsigned int k = 1; k <= 5; ++k) {
        DSGraph dsg(dsg_orig);
        add_ds::add_ds(dsg, k);
        std::vector<IVertex> spd_ord = spd::build_order(dsg);
        rgds::result_t res = rgds::rgds(dsg, setI(), std::list<setI>(), k,
                setI(), spd_ord, ncores);
        EXPECT_EQ(true, res.second) << "k = " << k;
        EXPECT_TRUE(helpers::is_ds(dsg, res.first)) << "k = " << k;
    }
}

TEST(addDS, known) {
    // add ds to known graph which is smaller than its original ds
    unsigned int k_g1 = 1;
    DSGraph g1 = Parser().parse_graph_int("graphs/g1.txt");
    add_ds::add_ds(g1, k_g1);
    std::vector<IVertex> spd_ord_g1 = spd::build_order(g1);
    rgds::result_t res_g1 = rgds::rgds(g1, setI(), std::list<setI>(), k_g1,
            setI(), spd_ord_g1, 1);

    EXPECT_EQ(true, res_g1.second);
    EXPECT_TRUE(helpers::is_ds(g1, res_g1.first));

    unsigned int k_g10 = 2;
    DSGraph g10 = Parser().parse_graph_int("graphs/g10.txt");
    add_ds::add_ds(g10, k_g10);
    std::vector<IVertex> spd_ord_g10 = spd::build_order(g10);
    rgds::result_t res_g10 = rgds::rgds(g10, setI(), std::list<setI>(), k_g10,
            setI(), spd_ord_g10, 1);

    EXPECT_EQ(true, res_g10.second);
    EXPECT_TRUE(helpers::is_ds(g10, res_g10.first));
}

}    // namespace
