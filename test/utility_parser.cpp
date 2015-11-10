#define BOOST_TEST_MODULE dsSolver

#include <list>
#include <boost/test/unit_test.hpp>

#include "../src/utility/Parser.hpp"
#include "../src/utility/ListHelpers.hpp"

using namespace std;

BOOST_AUTO_TEST_SUITE(utility)
BOOST_AUTO_TEST_SUITE(utility_parser)

BOOST_AUTO_TEST_CASE(parse_good_graph_int) {
    Parser p;
    parsed_graph<int> pg = p.parse_graph_int("g1.txt");

    // check vertices
    int arrv[6] = {1, 2, 3, 4, 5, 6};
    list<int> verts_should = ListHelpers<int>()
            .init_by_arr(arrv, sizeof(arrv)/sizeof(arrv[0]));
    BOOST_TEST(pg.vertices == verts_should);

    // check edges
    pair<int,int> arre[4] = {pair<int,int>(1, 3), pair<int,int>(2, 3),
            pair<int,int>(1, 5), pair<int,int>(3,6)};
    list< pair<int,int> > edges_should = ListHelpers< pair<int,int> >()
            .init_by_arr(arre, sizeof(arre)/sizeof(arre[0]));
    BOOST_TEST(pg.edges == edges_should);
}

BOOST_AUTO_TEST_CASE(parse_bad_verts_graph_int) {
    Parser p;
    BOOST_CHECK_THROW(p.parse_graph_int("test/graph_bad_verts.txt"),
            runtime_error);
}

BOOST_AUTO_TEST_CASE(parse_bad_edges_graph_int) {
    Parser p;
    BOOST_CHECK_THROW(p.parse_graph_int("test/graph_bad_edges.txt"),
            runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
