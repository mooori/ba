#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "rgds.hpp"
#include "spd.hpp"
#include "../types/dstypes.hpp"
#include "../utility/bfds.hpp"
#include "../utility/helpers.hpp"
#include "../utility/Parser.hpp"


int main(int argc, char* argv[]) {

    const char* help =
            "./dsSolver graph rgds k\n./dsSolver graph bf\n";

    if(argc < 2) { std::cout << help; return 0; }
    
    std::string method(argv[2]);

    if(method != "bf" && method != "rgds") { std::cout << help; return 0; }
    if(method == "bf" && argc != 3) { std::cout << help; return 0; }
    if(method == "rgds" && argc != 4) { std::cout << help; return 0; }
    

    typedef std::set<IVertex> setI;
    const char* gpath = argv[1];
    
    std::cout << "Parsing " << gpath << "...";
    DSGraph dsg = Parser().parse_graph_int(gpath);
    std::cout << "done, num_vertices = " << dsg.num_vertices() << "\n";
    
    
    if(method == "rgds") {
        unsigned int k = std::atoi(argv[3]);
        std::time_t start = std::time(nullptr);
        if(k <= 0) { 
            std::cout << "implement typ check, then allow for k=0, too\n"; 
            return 0;
        }
        
        std::cout << "Constructing spd_ord...";
        std::vector<IVertex> spd_ord = spd::build_order(dsg);
        std::cout << "done\n";

        std::cout << "Evaluating rgds...\n";
        rgds::result_t res = rgds::rgds(dsg, setI(), std::list<setI>(), k,
                setI(), spd_ord);

        if(res.second) {
            std::cout << "Found DS D of size " << res.first.size() << "\n";
            std::cout << "Compuation time: "
                    << std::time(nullptr) - start << " secs\n";
            helpers::print("D", res.first);
        } else {
            std::cout << "No DS found for k = " << k << "\n";
        }

        if(res.second) {
            std::cout << "Verifying RGDS's result...result correct? " <<
                    helpers::is_ds(dsg, res.first) << "\n";
        }
    }

    if(method == "bf") {
        std::time_t start = std::time(nullptr);
        std::cout << "Calculating D by brute force...";
        setI ds = bfds::get_min_ds(dsg);
        std::cout << "done\n";

        std::cout << "Domination number: " << ds.size() << "\n";
        std::cout << "Compuation time: "
                << std::time(nullptr) - start << " secs\n";
        helpers::print("D", ds);
    }

    
    return 0;
}
