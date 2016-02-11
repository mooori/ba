#include <iostream>
#include <set>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

#include "Distance.hpp"
#include "FuncIter.hpp"
#include "MinHS.hpp"
#include "SubsetGen.hpp"
#include "WComps.hpp"
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
    //std::cout << "fnumber[0] = " << f.get_digit_val(0) << std::endl;
    //std::cout << "fnumber[1] = " << f.get_digit_val(1) << std::endl;

    FuncIter f2(6, 3);
    //FuncIter f3(3, 0);

    std::vector<int> v;
    v.push_back(11); v.push_back(22); v.push_back(33); v.push_back(44);
    SubsetGen<int> sg(v);
    sg.get_subsets(4);
    //sg.print_ssets();
    sg.get_subsets(3);
    //sg.print_ssets();

    std::set<BVertex> W;
    WComps wc(dsg, W);
    std::list<DSGraph*>* comps = wc.get();
    WComps::delete_comps_ptrs(comps);

    DSGraph dsg1 = Parser().parse_graph_int("../test/graphs/g1.txt");
    std::set<BVertex> W1;
    std::cout << "about to build W\n";
    W1.insert(dsg1.get_BVertex(1)); W1.insert(dsg1.get_BVertex(4));
    W1.insert(dsg1.get_BVertex(5));
    std::cout << "built up W\n";
    WComps wc1(dsg1, W1);
    std::list<DSGraph*>* comps1 = wc1.get();
    WComps::delete_comps_ptrs(comps1);

    DSGraph dsg2 = Parser().parse_graph_int("../test/graphs/g_distance.txt");
    std::set<BVertex> W2{
        dsg2.get_BVertex(0), dsg2.get_BVertex(1), dsg2.get_BVertex(4),
        dsg2.get_BVertex(10), dsg2.get_BVertex(12)
    };
    WComps WC2(dsg2, W2);
    std::list<DSGraph*>* comps2 = WC2.get();
    WComps::delete_comps_ptrs(comps2);
    

    std::cout << "falou\n";
    return 0;
}
