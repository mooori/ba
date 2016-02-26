#include <random>
#include <set>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/main/rgds.hpp"
#include "../src/main/spd.hpp"
#include "../src/types/dstypes.hpp"
#include "../src/utility/rand_g.hpp"
#include "../src/utility/bfds.hpp"
#include "../src/utility/helpers.hpp"

/**
 * @brief On random graph, get dom_num by brute force, test if rgds accepts k.
 *
 * Create a random graph and calculate a minimum dominating set by brute force.
 * Since it's brute force, use **small** graphs. Then check if RGDS finds D
 * with |D| = k.
 *
 * To run it in a loop:
 *     ./test --gtest_filter=dsOnRandG* --gtest_repeat=xxx \
 *     --gtest_break_on_failure
 */

namespace {

typedef std::set<IVertex> setI;

// without preprocessing
TEST(dsOnRandG, dsOnRandG) {
    unsigned int max_nverts = 50;
    unsigned int ncores = 4;

    // setup random number generator for number of vertices
    // (taken from cppreference.com: std::uniform_int_distribution)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, max_nverts);

    unsigned int nverts = dis(gen);
    DSGraph dsg = rand_g::create(nverts);
    //std::cout << "Working on graph with num_vertices() = " << nverts << "\n";
    //std::cout << "num_edges = " << std::distance(dsg.edges().first,
    //        dsg.edges().second) << "\n";

    // get min dom set by brute force
    setI ds_bf = bfds::get_min_ds(dsg);
    //std::cout << "bf done\tdom_num = " << ds_bf.size() << "\n";

    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    rgds::result_t res = rgds::rgds(dsg, setI(), std::list<setI>(),
            ds_bf.size(), setI(), spd_ord, ncores);

    if(!res.second) {
        std::cout << "Failed on following input:\n";
        helpers::print(dsg);
        helpers::print("res.first", res.first);
    }

    ASSERT_TRUE(res.second);
    ASSERT_TRUE(helpers::is_ds(dsg, res.first));
    ASSERT_EQ(ds_bf.size(), res.first.size());
    //std::cout << "\tOK\n";
}

}    // namespace
