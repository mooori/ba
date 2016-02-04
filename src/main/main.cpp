#include <iostream>
#include <set>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

#include "Distance.hpp"
#include "FuncIter.hpp"
#include "../types/dstypes.hpp"
#include "../utility/Parser.hpp"
#include "../utility/setops.hpp"


int main() {

    Parser p;
    DSGraph dsg =
            p.parse_graph_int("../test/graphs/g1.txt");
    dsg.add_IVertex(666);

    FuncIter f(3, 4);
    f.increment();
    std::cout << "fnumber[0] = " << f.get_digit_val(0) << std::endl;
    std::cout << "fnumber[1] = " << f.get_digit_val(1) << std::endl;
    f.increment();
    f.increment();
    f.increment();
    std::cout << "fnumber[0] = " << f.get_digit_val(0) << std::endl;
    std::cout << "fnumber[1] = " << f.get_digit_val(1) << std::endl;

    FuncIter f2(0, 3);
    //FuncIter f3(3, 0);

    return 0;
}
