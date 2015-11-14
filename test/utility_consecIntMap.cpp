#include <list>

#include "gtest/gtest.h"

#include "../src/utility/ConsecIntMap.hpp"
#include "../src/utility/ListHelpers.hpp"

using namespace std;

namespace {

// test both init and lookup at once, since ConsecIntMap's `map_` shall
// not be exposed (and hence cannot be compared by `==` to other map)
// test for two ConsecIntMaps, cip1 constructed via cip(vals), cip2 via
//     cip() and then cip2.populate(vals)
TEST(ConsecIntMap, initAndLookups) {
    // build ConsecIntMap
    int keys_arr[3] = {13, 42, 666};
    list<int> keys = ListHelpers<int>().init_by_arr(keys_arr,
            sizeof(keys_arr)/sizeof(keys_arr[0]));
    ConsecIntMap<int> cip1(keys);
    ConsecIntMap<int> cip2;
    cip2.populate(keys);

    // correct key value pairs stored?
    for(unsigned int i = 0; i < sizeof(keys_arr)/sizeof(keys_arr[0]); ++i) {
        EXPECT_EQ(i, cip1.find(keys_arr[i]));
        EXPECT_EQ(i, cip2.find(keys_arr[i]));
    }

    // nothing else stored?
    EXPECT_EQ(sizeof(keys_arr)/sizeof(keys_arr[0]), cip1.size());
    EXPECT_EQ(sizeof(keys_arr)/sizeof(keys_arr[0]), cip2.size());

    // trying to find something not there throws error?
    EXPECT_THROW(cip1.find(1001), out_of_range);
    EXPECT_THROW(cip2.find(1001), out_of_range);
}

}
