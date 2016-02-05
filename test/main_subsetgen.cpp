#include <list>
#include <set>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "../src/main/SubsetGen.hpp"
#include "../src/utility/ListHelpers.hpp"


namespace {
typedef std::list< std::set<int> > list_set_t;

/** Compare two lists element by element */
void compare_lists(list_set_t should, list_set_t act) {
    list_set_t::iterator a_it = act.begin();
    for(list_set_t::iterator s_it = should.begin();
            s_it != should.end(); ++s_it) {
        ASSERT_FALSE(a_it == act.end()) << "too few elements in actual";
        EXPECT_EQ(*s_it, *a_it);
        ++a_it;
    }
    EXPECT_TRUE(a_it == act.end()) << "too much elements in actual";
    return;
}

TEST(SubsetGen, SubsetGen) {
    typedef std::list< std::set<int> > set_list_t;
    ListHelpers< std::set<int> > lh;

    // for { 11, 22, 33, 44 }, test for 0...4 generation of all subsets
    std::vector<int> pop;
    pop.push_back(11); pop.push_back(22);
    pop.push_back(33); pop.push_back(44);

    SubsetGen<int> sg(pop);

    // k = 0
    set_list_t l0 = sg.get_subsets(0);
    set_list_t should0;
    should0.push_back(std::set<int>());
    EXPECT_EQ(1, l0.size());
    compare_lists(should0, l0);

    // k = 1
    set_list_t l1 = sg.get_subsets(1);
    EXPECT_EQ(4, l1.size());
    std::set<int> s11, s12, s13, s14;
    s11.insert(11); s12.insert(22); s13.insert(33); s14.insert(44);
    std::set<int> arr1[4] = { s11, s12, s13, s14 };
    set_list_t should1 = lh.init_by_arr(arr1, sizeof(arr1)/sizeof(arr1[0]));
    compare_lists(should1, l1);

    // k = 2
    set_list_t l2 = sg.get_subsets(2);
    EXPECT_EQ(6, l2.size());
    std::set<int> s21, s22, s23, s24, s25, s26;
    s21.insert(11); s21.insert(22);
    s22.insert(11); s22.insert(33);
    s23.insert(11); s23.insert(44);
    s24.insert(22); s24.insert(33);
    s25.insert(22); s25.insert(44);
    s26.insert(33); s26.insert(44);
    std::set<int> arr2[6] = { s21, s22, s23, s24, s25, s26 };
    set_list_t should2 = lh.init_by_arr(arr2, sizeof(arr2)/sizeof(arr2[0]));
    compare_lists(should2, l2);

    // k = 3
    set_list_t l3 = sg.get_subsets(3);
    EXPECT_EQ(4, l3.size());
    std::set<int> s31, s32, s33, s34;
    s31.insert(11); s31.insert(22); s31.insert(33);
    s32.insert(11); s32.insert(22); s32.insert(44);
    s33.insert(11); s33.insert(33); s33.insert(44);
    s34.insert(22); s34.insert(33); s34.insert(44);
    std::set<int> arr3[4] = { s31, s32, s33, s34 };
    set_list_t should3 = lh.init_by_arr(arr3, sizeof(arr3)/sizeof(arr3[0]));
    compare_lists(should3, l3);

    // k = 4
    set_list_t l4 = sg.get_subsets(4);
    EXPECT_EQ(1, l4.size());
    std::set<int> s41;
    s41.insert(11); s41.insert(22); s41.insert(33); s41.insert(44);
    set_list_t should4; should4.push_back(s41);
    compare_lists(should4, l4);

    // values of k for which error should be thrown
    EXPECT_THROW(sg.get_subsets(5), std::runtime_error);
    EXPECT_THROW(sg.get_subsets(-666), std::runtime_error);
}

TEST(SubsetGen, emptyPopulation) {
    std::vector<int> pop;
    SubsetGen<int> sg(pop);

    list_set_t l0 = sg.get_subsets(0);
    EXPECT_EQ(1, l0.size());
    list_set_t should0; should0.push_back(std::set<int>());
    compare_lists(should0, l0);
}

} // namespace
