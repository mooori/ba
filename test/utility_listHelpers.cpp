#include <list>

#include <boost/assign/list_of.hpp> // to create a list
#include "gtest/gtest.h"

#include "../src/utility/ListHelpers.hpp"

using namespace std;

namespace {

TEST(ListHelpers, initInt) {
    list<int> l1, l2;
    int arr[6] = {1,2,3,4,5,6};
    l1 = ListHelpers<int>().init_by_arr(arr, sizeof(arr)/sizeof(arr[0]));
    l2 = boost::assign::list_of(1)(2)(3)(4)(5)(6);
    EXPECT_EQ(l2, l1);
}

} // namespace
