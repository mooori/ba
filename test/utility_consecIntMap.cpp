#include <list>

#include "gtest/gtest.h"

#include "../src/utility/ConsecIntMap.hpp"
#include "../src/utility/ListHelpers.hpp"

using namespace std;

namespace {

// test both init and lookup at once, since ConsecIntMap's `map_` shall
// not be exposed (and hence cannot be compared by `==` to other map)
TEST(ConsecIntMap, initAndLookups) {
    // build ConsecIntMap
    int keys_arr[3] = {13, 42, 666};
    list<int> keys = ListHelpers<int>().init_by_arr(keys_arr,
            sizeof(keys_arr)/sizeof(keys_arr[0]));
    ConsecIntMap<int> cip(keys);

    // correct key value pairs stored?
    for(unsigned int i = 0; i < sizeof(keys_arr)/sizeof(keys_arr[0]); ++i) {
        EXPECT_EQ(i, cip.find(keys_arr[i]));
    }

    // nothing else stored?
    EXPECT_EQ(sizeof(keys_arr)/sizeof(keys_arr[0]), cip.size());

    // trying to find something not there throws error?
    EXPECT_THROW(cip.find(1001), out_of_range);
}

}
