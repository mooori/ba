#include <iostream>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>

#include "../types/dstypes.hpp"
#include "../types/distance_info.hpp"
#include "../utility/Parser.hpp"
#include "ConnComps.hpp"
#include "Distance.hpp"

using namespace boost;

int main() {
    Parser p;
    p.hey();
    parsed_graph<int> pg = p.parse_graph_int(
            "/home/mo/docs/tu/ba/test/g_distance.txt");
    DSGraph g(pg.vertices.size());
    for(std::list< std::pair<int, int> >::iterator it = pg.edges.begin();
            it != pg.edges.end(); ++it) {
        std::pair<int, int> e = *it;
        add_edge(e.first, e.second, g);
    }
    //vector<TGraph*> conn_comps = ConnComps<TGraph>().find(g);
    distance_info di = Distance<DSGraph>().get_info(g);
    std::cout << "radius = " << di.radius << std::endl;
    std::cout << "diameter = " << di.diameter << std::endl;
    return 0;
}
