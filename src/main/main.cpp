#include <iostream>
#include <set>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

#include "Distance.hpp"
#include "FuncIter.hpp"
#include "MinHS.hpp"
#include "rgds.hpp"
#include "SubsetGen.hpp"
#include "WComps.hpp"
#include "../types/dstypes.hpp"
#include "../utility/helpers.hpp"
#include "../utility/Parser.hpp"
#include "../utility/setops.hpp"


int main() {

    Parser p;
    DSGraph dsg =
            p.parse_graph_int("../test/graphs/g1.txt");
    dsg.add_IVertex(666);

    FuncIter f(3, 4);
    f.increment();
    f.increment();
    f.increment();
    f.increment();

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
    W1.insert(dsg1.get_BVertex(1)); W1.insert(dsg1.get_BVertex(4));
    W1.insert(dsg1.get_BVertex(5));
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
    
    typedef std::set<IVertex> setI;
    DSGraph dsg3 = Parser().parse_graph_int("../test/graphs/g_distance.txt");
    rgds::result_t res3 = rgds::rgds(dsg3, setI(), std::list<setI>(), 4,
            setI());
    helpers::print("g_distance: D ", res3.first);

    /**
    DSGraph dsg4 = Parser().parse_graph_int("../test/graphs/g2.txt");
    rgds::result_t res4 = rgds::rgds(dsg4, setI(), std::list<setI>(), 2,
            setI());
    helpers::print("g2.txt D ", res4.first);
    */

    /**
    DSGraph dsg5 = Parser().parse_graph_int("../test/graphs/g3.txt");
    rgds::result_t res5 = rgds::rgds(dsg5, setI(), std::list<setI>(), 3,
            setI());
    helpers::print("g3.txt D ", res5.first);

    DSGraph dsg6 = Parser().parse_graph_int("../test/graphs/g4.txt");
    rgds::result_t res6 = rgds::rgds(dsg6, setI(), std::list<setI>(), 3,
            setI());
    helpers::print("g4.txt D ", res6.first);

    DSGraph dsg7 = Parser().parse_graph_int("../test/graphs/g5.txt");
    rgds::result_t res7 = rgds::rgds(dsg7, setI(), std::list<setI>(), 3,
            setI());
    helpers::print("g5.txt D ", res7.first);

    DSGraph dsg8 = Parser().parse_graph_int("../test/graphs/g6.txt");
    rgds::result_t res8 = rgds::rgds(dsg8, setI(), std::list<setI>(), 3,
            setI());
    helpers::print("g6.txt D ", res8.first);
    */

    /**
    DSGraph dsg9 = Parser().parse_graph_int("../test/graphs/empty.txt");
    rgds::result_t res9 = rgds::rgds(dsg9, setI(), std::list<setI>(), 0,
            setI());
    helpers::print("empty.txtt D ", res9.first);
    */

    DSGraph dsg10 = Parser().parse_graph_int("../test/graphs/g8.txt");
    rgds::result_t res10 = rgds::rgds(dsg10, setI(), std::list<setI>(), 3,
            setI());
    std::cout << "res10 bool = " << res10.second << "\n";
    helpers::print("g8.txt D ", res10.first);

    std::cout << "exiting\n";
    return 0;
}
