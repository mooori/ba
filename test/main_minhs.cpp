#include <list>
#include <set>

#include <gtest/gtest.h>

#include "../src/main/MinHS.hpp"

namespace{

TEST(MinHS, buildPop) {
    // If two vertices have same type, the one with lower id will be
    // added to population (as iteration is over universe, which is a set).
    //
    // For testing buildPop, max_k is not used, so use any value.
    typedef std::set<IVertex> setI;

    // 1
    setI F11{ 11, 22, 33 };
    setI F12{ 11, 22 };
    setI F13{ 33, 44 };
    MinHS hs1(std::list<setI>({ F11, F12, F13 }), 666);
    EXPECT_EQ(setI({ 11, 33, 44 }), hs1.build_pop());

    // 2
    setI F21{ 1, 3, 5 };
    setI F22{ 1, 2, 4 };
    setI F23{ 1, 4, 6 };
    setI F24{ 1, 5, 6 };
    setI F25{ 2, 4, 7 };
    setI F26{ 3, 5, 8 };
    MinHS hs2(std::list<setI>({ F21, F22, F23, F24, F25, F26 }), 666);
    EXPECT_EQ(setI({ 1, 2, 3, 4, 5, 6, 7, 8 }), hs2.build_pop());

    // 3
    setI F31{ 1, 3, 5, 8 };
    setI F32{ 1, 2, 4 };
    setI F33{ 1, 2, 4, 6 };
    setI F34{ 1, 5, 6 };
    setI F35{ 2, 4, 7 };
    setI F36{ 3, 5, 8 };
    MinHS hs3(std::list<setI>({ F31, F32, F33, F34, F35, F36 }), 666);
    EXPECT_EQ(setI({ 1, 2, 3, 5, 6, 7 }), hs3.build_pop());

    // 4
    setI F41{ 66, 77 };
    setI F42{ 66, 77 };
    setI F43{ 66, 77 };
    MinHS hs4(std::list<setI>({ F41, F42, F43, }), 666);
    EXPECT_EQ(setI({ 66 }), hs4.build_pop());

    // 5
    setI F51, F52;
    MinHS hs5(std::list<setI>({ F51, F52 }), 666);
    EXPECT_EQ(setI(), hs5.build_pop());

    // 6
    setI F6{ 8, 9, 10 };
    MinHS hs6(std::list<setI>({ F6 }), 666);
    EXPECT_EQ(setI({ 8 }), hs6.build_pop());
}

} //
