#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "PrePro1.hpp"
#include "PrePro2.hpp"
#include "rgds.hpp"
#include "spd.hpp"
#include "../types/dstypes.hpp"
#include "../utility/bfds.hpp"
#include "../utility/helpers.hpp"
#include "../utility/Parser.hpp"

int show_help_exit() {
    std::cout << "./dsSolver graph rgds pp0||pp1||pp2 k ncores \n./dsSolver graph bf pp0 bf\n";
    return 0;
}

int main(int argc, char* argv[]) {

    if(argc < 4) { return show_help_exit(); }
    
    std::string method(argv[2]);
    std::string pp(argv[3]);
    
    if(method == "bf" && argc != 4) { return show_help_exit(); }
    if(method == "rgds" && argc != 6) { return show_help_exit(); }

    if(method != "bf" && method != "rgds") { return show_help_exit(); }
    if(pp != "pp0" && pp != "pp1" && pp != "pp2") { return show_help_exit(); }

    if(method == "bf" && pp != "pp0") {
        std::cout << "Brute force only with pp0\n";
        return 0;
    }

    typedef std::set<IVertex> setI;
    const char* gpath = argv[1];
    setI pre_H, pre_D;
    
    std::cout << "Parsing " << gpath << "...";
    DSGraph dsg = Parser().parse_graph_int(gpath);
    DSGraph dsg_orig(dsg);    // for verification, as PrePro changes dsg
    std::cout << "done, num_vertices = " << dsg.num_vertices() << "\n";

    if(pp == "pp1") {
        std::time_t start_pp = std::time(nullptr);
        std::cout << "Apply Preprocessing Rule 1...";
        PrePro1 pp1(dsg);
        pp1.run();
        pre_H = pp1.pre_H;
        pre_D = pp1.pre_D;
        std::cout << "done; num_vertices remaining: " <<
                dsg.num_vertices() << "\n";
        std::cout << "pp time: " << std::time(nullptr) - start_pp << "\n";
    }
    else if(pp == "pp2") {
        std::time_t start_pp = std::time(nullptr);
        std::cout << "Apply Preprocessing Rule 2...";
        PrePro2 pp2(dsg);
        pp2.run();
        pre_H = pp2.pre_H;
        pre_D = pp2.pre_D;
        std::cout << "done; num_vertices remaining: " <<
                dsg.num_vertices() << "\n";
        std::cout << "pp time: " << std::time(nullptr) - start_pp << "\n";
    }
    
    if(method == "rgds") {
        unsigned int k = std::atoi(argv[4]);
        unsigned int ncores = std::atoi(argv[5]);
        std::time_t start = std::time(nullptr);
        if(k <= 0) { 
            std::cout << "implement type check, then allow for k=0, too\n"; 
            return 0;
        }

        // abort if |pre_D| > k
        if(pre_D.size() > k) {
            std::cout << "No DS found for k = " << k << "\n";
            std::cout << "Preprocessing: |D| > k\n";
            std::cout << "Computation time: "
                    << std::time(nullptr) - start << " secs\n";
            return 0;
        }
        
        std::cout << "Constructing spd_ord...";
        std::time_t start_spd = std::time(nullptr);
        std::vector<IVertex> spd_ord = spd::build_order(dsg);
        std::cout << "done\n";
        std::cout << "spd time: " << std::time(nullptr) - start_spd << "\n";

        std::cout << "Evaluating rgds...\n";
        rgds::result_t res = rgds::rgds(dsg, pre_H, std::list<setI>(),
                k - pre_D.size(), pre_D, spd_ord, ncores);

        if(res.second) {
            std::cout << "Found DS D of size " << res.first.size() << "\n";
            helpers::print("D", res.first);
        } else {
            std::cout << "No DS found for k = " << k << "\n";
        }

        if(res.second) {
            std::cout << "Verifying RGDS's result...result correct? " <<
                    helpers::is_ds(dsg_orig, res.first) << "\n";
        }

        std::cout << "Compuation time: "
                << std::time(nullptr) - start << " secs\n";
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
