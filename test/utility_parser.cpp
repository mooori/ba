#define BOOST_TEST_MODULE utility_parser

#include <boost/test/included/unit_test.hpp>

#include "/home/mo/docs/tu/ba/src/utility/Parser.hpp"

BOOST_AUTO_TEST_CASE(parse_good_graph) {
    Parser p;
    parsed_graph<int> pg = p.parse_graph_int("/home/mo/docs/tu/ba/g1.txt");
    BOOST_TEST(true);
}
