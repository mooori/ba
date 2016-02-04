#include <list>

#include <gtest/gtest.h>

#include "../src/utility/ListHelpers.hpp"

namespace {

TEST(ListHelpers, initInt) {
    std::list<int> l1, l2;
    int arr[6] = {1,2,3,4,5,6};
    l1 = ListHelpers<int>().init_by_arr(arr, sizeof(arr)/sizeof(arr[0]));
    l2.push_back(1); l2.push_back(2); l2.push_back(3);
    l2.push_back(4); l2.push_back(5); l2.push_back(6);
    EXPECT_EQ(l2, l1);
}

} // namespace
