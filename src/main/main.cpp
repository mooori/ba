#include <iostream>
#include <set>
#include <utility>

#include "rgds.hpp"
#include "spd.hpp"
#include "../types/dstypes.hpp"
#include "../utility/helpers.hpp"
#include "../utility/Parser.hpp"


int main() {

    typedef std::set<IVertex> setI;
    const char* gpath =
            //"../test/graphs/g_distance.txt";
            "../konect/adjnoun_adjacency/out.adjnoun_adjacency_adjacency";
    unsigned int k = 30;
    
    std::cout << "Parsing " << gpath << "...";
    DSGraph dsg = Parser().parse_graph_int(gpath);
    std::cout << "done, num_vertices = " << dsg.num_vertices() << "\n";
    
    std::cout << "Constructing spd_ord...";
    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    std::cout << "done\n";

    std::cout << "Evaluating rgds...\n";
    rgds::result_t res = rgds::rgds(dsg, setI(), std::list<setI>(), k,
            setI(), spd_ord);

    if(res.second) {
        std::cout << "Found DS D of size " << res.first.size() << "\n";
        helpers::print("D", res.first);
    } else {
        std::cout << "No DS found for k = " << k << "\n";
    }
    
    return 0;
}
