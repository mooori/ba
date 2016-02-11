#include <list>
#include <set>

#include <gtest/gtest.h>

#include "../src/main/MinHS.hpp"

namespace{

TEST(MinHS, get) {
    // get() returns the lexicographically lowest hitting set, because
    //   - if two vertices have same type, lower id vertex is added to pop
    //   - SubsetGen creates subsets in lexicographic order
    typedef std::set<IVertex> setI;
    typedef std::pair<std::set<IVertex>, bool> res_t;
    
    // 1
    setI F11{ 11, 22, 33 };
    setI F12{ 11, 22 };
    setI F13{ 33, 44 };
    std::list<setI> l1{ F11, F12, F13 };

    MinHS hs11(l1, 666);
    res_t res11 = hs11.get();
    EXPECT_EQ(true, res11.second);
    EXPECT_EQ(setI({ 11, 33 }), res11.first);
    
    MinHS hs12(l1, 2);
    res_t res12 = hs12.get();
    EXPECT_EQ(true, res12.second);
    EXPECT_EQ(setI({ 11, 33 }), res12.first);

    MinHS hs13(l1, 1);
    res_t res13 = hs13.get();
    EXPECT_EQ(false, res13.second);
    EXPECT_EQ(setI(), res13.first);

    // 2
    setI F21{ 1, 3, 5 };
    setI F22{ 1, 2, 4 };
    setI F23{ 1, 4, 6 };
    setI F24{ 1, 5, 6 };
    setI F25{ 2, 4, 7 };
    setI F26{ 3, 5, 8 };
    std::list<setI> l2{ F21, F22, F23, F24, F25, F26 };

    MinHS hs21(l2, 2);
    res_t res21 = hs21.get();
    EXPECT_EQ(true, res21.second);
    EXPECT_EQ(setI({ 4, 5 }), res21.first);

    MinHS hs22(l2, 1);
    res_t res22 = hs22.get();
    EXPECT_EQ(false, res22.second);
    EXPECT_EQ(setI(), res22.first);

    // 3
    setI F31{ 1, 3, 5, 8 };
    setI F32{ 1, 2, 4 };
    setI F33{ 1, 2, 4, 6 };
    setI F34{ 1, 5, 6 };
    setI F35{ 2, 4, 7 };
    setI F36{ 3, 5, 8 };
    std::list<setI> l3{ F31, F32, F33, F34, F35, F36 };

    MinHS hs31(l3, 2);
    res_t res31 = hs31.get();
    EXPECT_EQ(true, res31.second);
    EXPECT_EQ(setI({ 2, 5 }), res31.first);

    MinHS hs32(l3, 1);
    res_t res32 = hs32.get();
    EXPECT_EQ(false, res32.second);
    EXPECT_EQ(setI(), res32.first);

    // 4
    setI F41{ 66, 77 };
    setI F42{ 66, 77 };
    setI F43{ 66, 77 };
    std::list<setI> l4{ F41, F42, F43, };

    MinHS hs41(l4, 1);
    res_t res41 = hs41.get();
    EXPECT_EQ(true, res41.second);
    EXPECT_EQ(setI({ 66 }), res41.first);

    MinHS hs42(l4, 0);
    res_t res42 = hs42.get();
    EXPECT_EQ(false, res42.second);
    EXPECT_EQ(setI(), res42.first);

    // 5
    setI F5{ 8, 9, 10 };
    std::list<setI> l5{ F5 };
    
    MinHS hs51(l5, 1);
    res_t res51 = hs51.get();
    EXPECT_EQ(true, res51.second);
    EXPECT_EQ(setI({ 8 }), res51.first);

    MinHS hs52(l5, 0);
    res_t res52 = hs52.get();
    EXPECT_EQ(false, res52.second);
    EXPECT_EQ(setI(), res52.first);

    // 6
    setI F61{ 1, 3, 5 };
    setI F62{ 1, 2, 4 };
    setI F63{ 1, 4, 6 };
    setI F64{ 1, 5, 6 };
    setI F65{ 2, 4, 7 };
    setI F66{ 3, 5, 8 };
    setI F67{ 9 };
    std::list<setI> l6{ F61, F62, F63, F64, F65, F66, F67 };

    MinHS hs61(l6, 3);
    res_t res61 = hs61.get();
    EXPECT_EQ(true, res61.second);
    EXPECT_EQ(setI({ 4, 5, 9 }), res61.first);

    MinHS hs62(l6, 2);
    res_t res62 = hs62.get();
    EXPECT_EQ(false, res62.second);
    EXPECT_EQ(setI(), res62.first);
}

TEST(MinHS, getEdgeCases) {
    typedef std::set<IVertex> setI;
    typedef std::pair<std::set<IVertex>, bool> res_t;

    // empty list of colors
    std::list<setI> l1;
    MinHS hs11(l1, 666);
    res_t res11 = hs11.get();
    EXPECT_EQ(true, res11.second);
    EXPECT_EQ(setI(), res11.first);

    MinHS hs12(l1, 0);
    res_t res12 = hs12.get();
    EXPECT_EQ(true, res12.second);
    EXPECT_EQ(setI(), res11.first);

    // all colors in list empty
    setI F21, F22, F23;
    std::list<setI> l2{ F21, F22, F23 };

    MinHS hs21(l2, 666);
    res_t res21 = hs21.get();
    EXPECT_EQ(true, res21.second);
    EXPECT_EQ(setI(), res21.first);

    MinHS hs22(l2, 0);
    res_t res22 = hs22.get();
    EXPECT_EQ(true, res22.second);
    EXPECT_EQ(setI(), res22.first);

    // k=0 but population != 0
    setI F31{ 13, 42 };
    std::list<setI> l3{ F31 };
    MinHS hs31(l3, 0);
    res_t res31 = hs31.get();
    EXPECT_EQ(false, res31.second);
    EXPECT_EQ(setI(), res31.first);

    // k < |minHS|
    setI F41{ 11, 22, 33 };
    setI F42{ 11, 22 };
    setI F43{ 33, 44 };
    std::list<setI> l4{ F41, F42, F43 };
    MinHS hs41(l4, 1);
    res_t res41 = hs41.get();
    EXPECT_EQ(false, res41.second);
    EXPECT_EQ(setI(), res41.first);
}
    

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

    // 7
    MinHS hs7(std::list<setI>(), 666);
    EXPECT_EQ(setI(), hs7.build_pop());
}

} //
