#include <iostream>

#include<boost/config.hpp>
#include<boost/graph/adjacency_list.hpp>

#include "types/dstypes.hpp"
#include "utility/Parser.hpp"
#include "ConnComps.hpp"

using namespace boost;

int main() {
    Parser p;
    p.hey();
    parsed_graph<int> pg = p.parse_graph_int("/home/mo/docs/tu/ba/g1.txt");
    //typedef adjacency_list<setS, listS, undirectedS> Graph;
    DSGraph g(pg.vertices.size());
    for(std::list< pair<int, int> >::iterator it = pg.edges.begin();
            it != pg.edges.end(); ++it) {
        pair<int, int> e = *it;
        add_edge(e.first, e.second, g);
    }
    vector<DSGraph*> conn_comps = ConnComps<DSGraph>().find(g);
    cout << num_vertices(g) << " = number of verts in g" << endl;
    //std::pair<int, int> e1 = pg.edges.front();
    //add_edge(e1.first, e1.second, g);
    /*
    std::cout << pg.vertices.front() << std::endl;
    pair<int,int> e1 = pg.edges.front();
    std::cout << e1.first << "," << e1.second << endl;
    */
    return 0;
}
