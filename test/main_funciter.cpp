#include <gtest/gtest.h>

#include "../src/main/FuncIter.hpp"

namespace {

TEST(FuncIter, throws) {
    EXPECT_THROW(FuncIter(0, 1), std::runtime_error); 
    EXPECT_THROW(FuncIter(0, 0), std::runtime_error);
}

TEST(FuncIter, FuncIter) {
    FuncIter f1(8, 3);
    EXPECT_EQ(false, f1.max_reached());

    // increment 129 times, fnumber should then be (129)_10 = (201)_8
    for(int i = 0; i < 129; ++i) {
        f1.increment();
        EXPECT_EQ(false, f1.max_reached());
    }
    EXPECT_EQ(1, f1.get_digit_val(0));
    EXPECT_EQ(0, f1.get_digit_val(1));
    EXPECT_EQ(2, f1.get_digit_val(2));

    // increment another 248 times, expect fnumber (377)_10 = (571)_8
    for(int i = 0; i < 248; ++i) {
        f1.increment();
        EXPECT_EQ(false, f1.max_reached());
    }
    EXPECT_EQ(1, f1.get_digit_val(0));
    EXPECT_EQ(7, f1.get_digit_val(1));
    EXPECT_EQ(5, f1.get_digit_val(2));

    // expect fnumber (510)_10 = (776)_8
    for(int i = 0; i < 133; ++i) {
        f1.increment();
        EXPECT_EQ(false, f1.max_reached());
    }
    EXPECT_EQ(6, f1.get_digit_val(0));
    EXPECT_EQ(7, f1.get_digit_val(1));
    EXPECT_EQ(7, f1.get_digit_val(2));

    // get to max fnumber
    f1.increment();
    EXPECT_EQ(true, f1.max_reached());
    EXPECT_THROW(f1.increment(), std::runtime_error);

    FuncIter f2(13, 4);
    // increment to max fnumber - 1 = (13^4) - 1 - 1
    for(int i = 0; i < pow(13, 4) - 2; ++i) {
        f2.increment();
        EXPECT_EQ(false, f2.max_reached());
    }
    EXPECT_EQ(11, f2.get_digit_val(0));
    EXPECT_EQ(12, f2.get_digit_val(1));
    EXPECT_EQ(12, f2.get_digit_val(2));
    EXPECT_EQ(12, f2.get_digit_val(3));

    f2.increment();
    EXPECT_EQ(true, f2.max_reached());
    EXPECT_THROW(f2.increment(), std::runtime_error);

    FuncIter f3(16, 5);
    // increment to (7788)_10 = (1E6C)_16
    for(int i = 0; i < 7788; ++i) {
        f3.increment();
        EXPECT_EQ(false, f3.max_reached());
    }
    EXPECT_EQ(12, f3.get_digit_val(0));
    EXPECT_EQ(6, f3.get_digit_val(1));
    EXPECT_EQ(14, f3.get_digit_val(2));
    EXPECT_EQ(1, f3.get_digit_val(3));
    EXPECT_EQ(0, f3.get_digit_val(4));

    for(int i = 0; i < pow(16, 5) - 7788 - 1 - 1; ++i) {
        f3.increment();
        EXPECT_EQ(false, f3.max_reached());
    }
    EXPECT_EQ(14, f3.get_digit_val(0));
    EXPECT_EQ(15, f3.get_digit_val(1));
    EXPECT_EQ(15, f3.get_digit_val(2));
    EXPECT_EQ(15, f3.get_digit_val(3));
    EXPECT_EQ(15, f3.get_digit_val(4));

    f3.increment();
    EXPECT_EQ(true, f3.max_reached());
    EXPECT_THROW(f3.increment(), std::runtime_error);
}

TEST(FuncIter, edgeCases) {
    // when radix = 1, there are no comps, only backland
    // so backland must handle all F colors
    // max reached at init, no increment() possible
    FuncIter f(1, 666);
    for(int i = 0; i < 666; ++i) { EXPECT_EQ(0, f.get_digit_val(i)); }
    EXPECT_EQ(true, f.max_reached());
    EXPECT_THROW(f.increment(), std::runtime_error);

    // when n = 1, there's just one F color
    FuncIter f2(4, 1);
    f2.increment();
    EXPECT_EQ(1, f2.get_digit_val(0));
    EXPECT_EQ(false, f2.max_reached());
    f2.increment();
    EXPECT_EQ(2, f2.get_digit_val(0));
    EXPECT_EQ(false, f2.max_reached());
    f2.increment();
    EXPECT_EQ(3, f2.get_digit_val(0));
    EXPECT_EQ(true, f2.max_reached());
    EXPECT_THROW(f2.increment(), std::runtime_error);

    // when n = 0, there are no F colors
    // must be possible to init though
    FuncIter f3(7, 0);
    EXPECT_EQ(true, f3.max_reached());
    EXPECT_THROW(f.increment(), std::runtime_error);
}

} // namespace
