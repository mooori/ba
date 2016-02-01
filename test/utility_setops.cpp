#include <list>
#include <set>

#include <gtest/gtest.h>

#include "../src/utility/setops.hpp"

namespace {

// fixture class
class setopsTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // empty = { }
        // s1 = { 13, 666 }
        s1.insert(13); s1.insert(666);
        s1_orig = s1;

        // s2 = { 21, 23, 666 }
        s2.insert(21); s2.insert(23); s2.insert(666);
        s2_orig = s2;

        // s3 = { 7 }
        s3.insert(7);
        s3_orig = s3;

        // s4 = { 21, 23, 777 }
        s4.insert(21); s4.insert(23); s4.insert(777);
        s4_orig = s4;

        // s5 = { 21, 666 }
        s5.insert(21); s5.insert(666);
        s5_orig = s5;
    }

    std::set<int> empty;
    std::set<int> s1;
    std::set<int> s2;
    std::set<int> s3;
    std::set<int> s4;
    std::set<int> s5;
    std::set<int> s1_orig;
    std::set<int> s2_orig;
    std::set<int> s3_orig;
    std::set<int> s4_orig;
    std::set<int> s5_orig;
};

TEST_F(setopsTest, Union) {
    // _new
    EXPECT_EQ(s1_orig, setops::union_new(s1, empty));
    EXPECT_EQ(s2_orig, setops::union_new(empty, s2));
    EXPECT_EQ(std::set<int>(), setops::union_new(empty, empty));
    EXPECT_EQ(s3_orig, setops::union_new(s3, s3));

    std::set<int> res1;
    res1.insert(13); res1.insert(21); res1.insert(23); res1.insert(666);
    EXPECT_EQ(res1, setops::union_new(s1, s2));
    EXPECT_EQ(res1, setops::union_new(s2, s1));

    std::set<int> res2;
    res2.insert(13); res2.insert(666); res2.insert(7);
    EXPECT_EQ(res2, setops::union_new(s1, s3));
    EXPECT_EQ(res2, setops::union_new(s3, s1));

    // make sure _new funcs didn't modify input
    EXPECT_EQ(s1_orig, s1);
    EXPECT_EQ(s2_orig, s2);
    EXPECT_EQ(s3_orig, s3);

    // _inp
    std::set<int> empty_ = empty;
    std::set<int> s1_ = s1;
    std::set<int> s2_ = s2;
    std::set<int> s3_ = s3;
    EXPECT_EQ(s1_orig, setops::union_inp(s1_, empty_));
    EXPECT_EQ(s2_orig, setops::union_inp(empty_, s2_));
    EXPECT_EQ(std::set<int>(), setops::union_inp(empty_, empty_));
    EXPECT_EQ(s3_orig, setops::union_inp(s3_, s3_));

    s1_ = s1; s2_ = s2;
    std::set<int> s1__ = s1; std::set<int> s2__ = s2;
    EXPECT_EQ(res1, setops::union_inp(s1_, s2_));
    EXPECT_EQ(res1, setops::union_inp(s2__, s1__));

    s1_ = s1; s1__ = s1; s3_ = s3; 
    std::set<int> s3__ = s3;
    EXPECT_EQ(res2, setops::union_inp(s1_, s3_));
    EXPECT_EQ(res2, setops::union_inp(s3__, s1__));
}

TEST_F(setopsTest, inters) {
    // _new
    EXPECT_EQ(empty, setops::inters_new(s1, empty));
    EXPECT_EQ(empty, setops::inters_new(empty, s2));
    EXPECT_EQ(std::set<int>(), setops::inters_new(empty, empty));
    EXPECT_EQ(s1_orig, setops::inters_new(s1, s1));

    EXPECT_EQ(std::set<int>(), setops::inters_new(s1, s4));
    EXPECT_EQ(std::set<int>(), setops::inters_new(s4, s1));

    std::set<int> res1;
    res1.insert(21); res1.insert(23);
    EXPECT_EQ(res1, setops::inters_new(s2, s4));
    EXPECT_EQ(res1, setops::inters_new(s4, s2));

    // make sure _new funcs didn't modify input
    EXPECT_EQ(s1_orig, s1);
    EXPECT_EQ(s2_orig, s2);
    EXPECT_EQ(s3_orig, s3);
    EXPECT_EQ(s4_orig, s4);

    // _inp
    std::set<int> empty_ = empty;
    std::set<int> s1_ = s1;
    std::set<int> s2_ = s2;
    std::set<int> s3_ = s3;
    std::set<int> s4_ = s4;
    EXPECT_EQ(empty, setops::inters_inp(s1_, empty_));
    EXPECT_EQ(empty, setops::inters_inp(empty_, s2_));
    EXPECT_EQ(std::set<int>(), setops::inters_inp(empty_, empty_));
    EXPECT_EQ(s4_orig, setops::inters_inp(s4_, s4_));

    s1_ = s1; s4_ = s4;
    std::set<int> s1__ = s1; std::set<int> s4__ = s4;
    EXPECT_EQ(std::set<int>(), setops::inters_inp(s1_, s4_));
    EXPECT_EQ(std::set<int>(), setops::inters_inp(s4__, s1__));

    s2_ = s2; s4_ = s4; s4__ = s4;
    std::set<int> s2__ = s2;
    EXPECT_EQ(res1, setops::inters_inp(s2_, s4_));
    EXPECT_EQ(res1, setops::inters_inp(s4__, s2));
}

TEST_F(setopsTest, setminus) {
    // _new
    EXPECT_EQ(s1_orig, setops::setminus_new(s1, empty));
    EXPECT_EQ(std::set<int>(), setops::setminus_new(s2, s2));
    EXPECT_EQ(std::set<int>(), setops::setminus_new(empty, s3));
    EXPECT_EQ(std::set<int>(), setops::setminus_new(empty, empty));

    std::set<int> res1;
    res1.insert(23); res1.insert(777);
    EXPECT_EQ(res1, setops::setminus_new(s4, s5));

    std::set<int> res2;
    res2.insert(666);
    EXPECT_EQ(res2, setops::setminus_new(s5, s4));
    EXPECT_EQ(res2, setops::setminus_new(s2, s4));

    EXPECT_EQ(s3_orig, setops::setminus_new(s3, s1));
    EXPECT_EQ(s1_orig, setops::setminus_new(s1, s3));

    // make sure _new funcs didn't modify input
    EXPECT_EQ(s1_orig, s1);
    EXPECT_EQ(s2_orig, s2);
    EXPECT_EQ(s3_orig, s3);
    EXPECT_EQ(s4_orig, s4);
    EXPECT_EQ(s5_orig, s5);

    // _inp
    std::set<int> empty_ = empty;
    std::set<int> s1_ = s1;
    std::set<int> s2_ = s2;
    std::set<int> s3_ = s3;
    std::set<int> s4_ = s4;
    std::set<int> s5_ = s5;

    EXPECT_EQ(s1_orig, setops::setminus_inp(s1_, empty_));
    EXPECT_EQ(std::set<int>(), setops::setminus_inp(s2_, s2_));
    EXPECT_EQ(std::set<int>(), setops::setminus_inp(empty_, s3_));
    EXPECT_EQ(std::set<int>(), setops::setminus_inp(empty_, empty_));

    s4_ = s4; s5_ = s5;
    EXPECT_EQ(res1, setops::setminus_inp(s4_, s5_));

    s2_ = s2; s4_ = s4; s5_ = s5;
    std::set<int> s4__ = s4;
    EXPECT_EQ(res2, setops::setminus_inp(s5_, s4_));
    EXPECT_EQ(res2, setops::setminus_inp(s2_, s4__));

    s1_ = s1; s3_ = s3;
    EXPECT_EQ(s3_orig, setops::setminus_inp(s3_, s1_));
    s1_ = s1; s3_ = s3;
    EXPECT_EQ(s1_orig, setops::setminus_inp(s1_, s3_));
}

TEST_F(setopsTest, filterContainingV) {
    typedef std::list< std::set<int> > list_set_t; 
    
    // _new
    // l1 = { empty, empty }
    list_set_t l1, l1_orig;
    l1.push_back(empty); l1.push_back(empty);
    l1_orig = l1;
    list_set_t l1_mod = setops::filter_containing_v_new(l1, 666);
    EXPECT_EQ(2, l1_mod.size());
    EXPECT_EQ(l1_orig, l1_mod);

    // l2 = { }
    list_set_t l2;
    list_set_t l2_mod = setops::filter_containing_v_new(l2, 666);
    EXPECT_EQ(0, l2_mod.size());
    EXPECT_EQ(list_set_t(), l2_mod);

    // l3 = { s1, s2, s3 }
    list_set_t l3, l3_orig;
    l3.push_back(s1); l3.push_back(s2); l3.push_back(s3);
    l3_orig = l3;
    list_set_t l3_mod = setops::filter_containing_v_new(l3, 666);
    list_set_t should3; should3.push_back(s3);
    EXPECT_EQ(1, l3_mod.size());
    EXPECT_EQ(should3, l3_mod);

    // remove sth that isn't there
    list_set_t l3_mod2 = setops::filter_containing_v_new(l3, 777);
    EXPECT_EQ(3, l3_mod2.size());
    EXPECT_EQ(l3_orig, l3_mod2);

    // l4 = { s4, s5 }
    list_set_t l4, l4_orig;
    l4.push_back(s4); l4.push_back(s5);
    l4_orig = l4;
    list_set_t l4_mod = setops::filter_containing_v_new(l4, 21);
    EXPECT_EQ(0, l4_mod.size());
    EXPECT_EQ(list_set_t(), l4_mod);

    // make sure _new funcs didn't modify input
    EXPECT_EQ(l1_orig, l1);
    EXPECT_EQ(list_set_t(), l2);
    EXPECT_EQ(l3_orig, l3);
    EXPECT_EQ(l4_orig, l4);

    // _inp, may modify l1,...
    // l1
    setops::filter_containing_v_inp(l1, 666);
    EXPECT_EQ(2, l1.size());
    EXPECT_EQ(l1_orig, l1);

    // l2
    setops::filter_containing_v_inp(l2, 666);
    EXPECT_EQ(0, l2.size());
    EXPECT_EQ(list_set_t(), l2);

    // l3
    setops::filter_containing_v_inp(l3, 666);
    EXPECT_EQ(1, l3.size());
    EXPECT_EQ(should3, l3);
    EXPECT_FALSE(l3 == l3_orig);

    l3 = l3_orig;
    setops::filter_containing_v_inp(l3, 777);
    EXPECT_EQ(3, l3.size());
    EXPECT_EQ(l3_orig, l3);

    // l4
    setops::filter_containing_v_inp(l4, 21);
    EXPECT_EQ(0, l4.size());
    EXPECT_EQ(list_set_t(), l4);
    EXPECT_FALSE(l4 == l4_orig);
}

} // namespace
