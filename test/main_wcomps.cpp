#include <algorithm>
#include <list>
#include <set>

#include <gtest/gtest.h>

#include "../src/main/DSGraph.hpp"
#include "../src/utility/Parser.hpp"
#include "../src/main/WComps.hpp"

namespace {

TEST(WComps, WComps) {
    // order of components in list returned by WComps::get() can be
    // inferred, as components are initiated with W.begin() as first vertex

    typedef std::set<IVertex> setI;

    // g1.txt; W = { 1, 4, 5 }
    DSGraph dsg1 = Parser().parse_graph_int("graphs/g1.txt");
    std::set<BVertex> W1{
        dsg1.get_BVertex(1), dsg1.get_BVertex(4), dsg1.get_BVertex(5)
    };
    WComps WC1(dsg1, W1);
    std::list<DSGraph*>* comps1 = WC1.get();
    EXPECT_EQ(2, comps1->size());

    // check components
    // C11, vertices
    std::list<DSGraph*>::iterator C_it1 = comps1->begin();
    DSGraph C11 = **C_it1;
    EXPECT_EQ(3, C11.num_vertices());
    EXPECT_EQ(setI({ 1, 3, 5 }), C11.get_set_IVertices());

    // C11, edges
    EXPECT_EQ(setI({ 3, 5 }), C11.get_adj_IVertices(1));
    EXPECT_EQ(setI({ 1, 5 }), C11.get_adj_IVertices(3));
    EXPECT_EQ(setI({ 1, 3 }), C11.get_adj_IVertices(5));

    // C12, vertices
    DSGraph C12 = **(++C_it1); 
    EXPECT_EQ(2, C12.num_vertices());
    EXPECT_EQ(setI({ 0, 4 }), C12.get_set_IVertices());

    // C12, edges
    EXPECT_EQ(setI({ 4 }), C12.get_adj_IVertices(0));
    EXPECT_EQ(setI({ 0 }), C12.get_adj_IVertices(4));

    //WComps::delete_comps_ptrs(comps1);

    // g_distance; W = { 0, 1, 4, 10, 12 }
    DSGraph dsg2 = Parser().parse_graph_int("graphs/g_distance.txt");
    std::set<BVertex> W2{
        dsg2.get_BVertex(0), dsg2.get_BVertex(1), dsg2.get_BVertex(4),
        dsg2.get_BVertex(10), dsg2.get_BVertex(12)
    };
    WComps WC2(dsg2, W2);
    std::list<DSGraph*>* comps2 = WC2.get();
    EXPECT_EQ(2, comps2->size());

    // C21, vertices
    std::list<DSGraph*>::iterator C_it2 = comps2->begin();
    DSGraph C21 = **C_it2;
    EXPECT_EQ(7, C21.num_vertices());
    EXPECT_EQ(setI({ 0, 1, 2, 3, 4, 5, 7 }), C21.get_set_IVertices());

    // C21, edges
    EXPECT_EQ(setI({ 1 }), C21.get_adj_IVertices(0));
    EXPECT_EQ(setI({ 0, 2, 4 }), C21.get_adj_IVertices(1));
    EXPECT_EQ(setI({ 1, 4 }), C21.get_adj_IVertices(2));
    EXPECT_EQ(setI({ 4, 7 }), C21.get_adj_IVertices(3));
    EXPECT_EQ(setI({ 1, 2, 3, 5, 7 }), C21.get_adj_IVertices(4));
    EXPECT_EQ(setI({ 4 }), C21.get_adj_IVertices(5));
    EXPECT_EQ(setI({ 3, 4 }), C21.get_adj_IVertices(7));

    // C22, vertices
    DSGraph C22 = **(++C_it2);
    EXPECT_EQ(4, C22.num_vertices());
    EXPECT_EQ(setI({ 9, 10, 11, 12 }), C22.get_set_IVertices());

    // C22, edges
    EXPECT_EQ(setI({ 11, 12 }), C22.get_adj_IVertices(9));
    EXPECT_EQ(setI({ 11 }), C22.get_adj_IVertices(10));
    EXPECT_EQ(setI({ 9, 10, 12 }), C22.get_adj_IVertices(11));
    EXPECT_EQ(setI({ 9, 11 }), C22.get_adj_IVertices(12));

    //WComps::delete_comps_ptrs(comps2);

    // g3.txt, W = { 777 }
    DSGraph dsg3 = Parser().parse_graph_int("graphs/g3.txt");
    std::set<BVertex> W3{ dsg3.get_BVertex(777) };
    WComps WC3(dsg3, W3);
    std::list<DSGraph*>* comps3 = WC3.get();
    EXPECT_EQ(1, comps3->size());

    // C3 edges and vertices
    DSGraph C3 = **(comps3->begin());
    EXPECT_EQ(1, C3.num_vertices());
    EXPECT_EQ(setI({ 777 }), C3.get_set_IVertices());
    EXPECT_EQ(setI(), C3.get_adj_IVertices(777));
}

TEST(WComps, edgeCases) {
    typedef std::set<IVertex> setI;

    // Only one comp, as W = V(G)
    // g2.txt; W = V(g2.txt)
    DSGraph dsg1 = Parser().parse_graph_int("graphs/g2.txt");
    std::set<BVertex> W1 = dsg1.get_set_BVertices();
    WComps WC1(dsg1, W1);
    std::list<DSGraph*>* comps1 = WC1.get();
    EXPECT_EQ(1, comps1->size());

    // C1 vertices
    DSGraph C1 = **(comps1->begin());
    EXPECT_EQ(6, C1.num_vertices());
    EXPECT_EQ(dsg1.get_set_IVertices(), C1.get_set_IVertices());

    // C1 edges
    EXPECT_EQ(setI({ 4 }), C1.get_adj_IVertices(0));
    EXPECT_EQ(setI({ 3, 4, 5 }), C1.get_adj_IVertices(1));
    EXPECT_EQ(setI({ 3 }), C1.get_adj_IVertices(2));
    EXPECT_EQ(setI({ 1, 2, 5 }), C1.get_adj_IVertices(3));
    EXPECT_EQ(setI({ 0, 1 }), C1.get_adj_IVertices(4));
    EXPECT_EQ(setI({ 1, 3 }), C1.get_adj_IVertices(5));

    // No comp, as W = {}
    // g1.txt; W = {}
    DSGraph dsg2 = Parser().parse_graph_int("graphs/g1.txt");
    std::set<BVertex> W2;
    WComps WC2(dsg2, W2);
    std::list<DSGraph*>* comps2 = WC2.get();
    EXPECT_EQ(0, comps2->size());

    // empty.txt, W = {}
    DSGraph dsg3 = Parser().parse_graph_int("graphs/empty.txt");
    std::set<BVertex> W3;
    WComps WC3(dsg3, W3);
    std::list<DSGraph*>* comps3 = WC3.get();
    EXPECT_EQ(0, comps3->size());
}

} // namespace
