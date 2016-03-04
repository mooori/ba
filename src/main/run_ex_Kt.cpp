#include <ctime>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <stdexcept>
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
#include "../utility/rand_g_ex_Kt.hpp"

/**
 * @brief Run rgds on random graphs excluding K_t to get statistics.
 *
 * For each graph calling rgds with k = |V(G)|
 */

int show_help_exit() {
    std::cout << "./runExKt pp0||pp1||pp2 t ncores\n";
    return 0;
}

/**
 * @param pp pp0||pp1||pp2, pp0 indicates no preprocessing
 * @param t which K_t to exclude
 * @param ncores number of threads that may be started by rgds
 */
void run(std::string pp, unsigned int t, unsigned int ncores);

/**
 * Do stuff for a single graph
 * @param nverts number of vertices of graph
 * @param ttotals total times spent for activities pp, spd and rgds
 * @param outf file to stream to
 */
void run_graph(unsigned int nverts, std::string pp, unsigned int t,
        unsigned int ncores, struct ttotals_t& ttotals, std::ofstream& outf);

/** Get number of seconds since start */
std::time_t secs_since(std::time_t start) {
    return std::time(nullptr) - start;
}

int main(int argc, char* argv[]) {

    if(argc < 4) { return show_help_exit(); }

    // check PrePro parameter
    std::string pp(argv[1]);
    if(pp != "pp0" && pp != "pp1" && pp != "pp2") { return show_help_exit(); }
    
    // check parameters t and ncores
    unsigned int t = std::atoi(argv[2]);
    unsigned int ncores = std::atoi(argv[3]);
    if(t <= 0 || ncores <= 0) { return show_help_exit(); }

    run(pp, t, ncores);

    return 0;
}

/** Hold total computation times for different tasks per nverts iteration */
struct ttotals_t {
    std::time_t pp;
    std::time_t spd;
    std::time_t rgds;
    ttotals_t() : pp(0), spd(0), rgds(0) { }
};

void run(std::string pp, unsigned int t, unsigned int ncores) {
    std::list<unsigned int> nverts{
        10, 100, 500, 1000, 5000, 10000, 20000, 50000, 80000, 100000  
    };
    unsigned int ngraphs = 100;

    for(std::list<unsigned int>::iterator it = nverts.begin();
            it != nverts.end(); ++it) {
        // open file
        std::ofstream outf((const char*) "../stats/nv_" + std::to_string(*it));
        if(!outf) {
            throw std::runtime_error("failed to open file for " + *it);
        }

        std::time_t start_nverts = std::time(nullptr);
        struct ttotals_t ttotals;

        for(unsigned int i = 0; i < ngraphs; ++i) {
            std::cout << "Graph " << i << " for nverts = " << *it << "\n";
            outf << "Graph " << i << "\n";
            run_graph(*it, pp, t, ncores, ttotals, outf);
        }

        outf << "\n\n" << std::string(80, '=') << "\n";
        outf << "Total times are:\n";
        outf << "nverts = " << *it << "\ttime: " << secs_since(start_nverts)
                << "\n";
        outf << "pp time: " << ttotals.pp << " for pp = " << pp << "\n";
        outf << "spd time: " << ttotals.spd << "\n";
        outf << "rgds time: " << ttotals.rgds << "\n";
        outf.close();
    }
}

void run_graph(unsigned int nverts, std::string pp, unsigned int t,
        unsigned int ncores, struct ttotals_t& ttotals, std::ofstream& outf) {
    DSGraph dsg = rand_g_ex_Kt::create(nverts, t);
    DSGraph dsg_orig(dsg);    // for verification, as PrePro changes dsg
    std::set<IVertex> pre_H, pre_D;

    // preprocessing
    std::time_t start_pp = std::time(nullptr);
    if(pp == "pp1") {
        PrePro1 pp1(dsg);
        pp1.run();
        pre_H = pp1.pre_H;
        pre_D = pp1.pre_D;
    }

    if(pp == "pp2") {
        PrePro2 pp2(dsg);
        pp2.run();
        pre_H = pp2.pre_H;
        pre_D = pp2.pre_D;
    }

    if(pp == "pp1" || pp == "pp2") {    // time & printing for both identical
        std::time_t pp_secs = secs_since(start_pp);
        ttotals.pp += pp_secs;
        outf << "\ttime pp: " << pp_secs << " for pp = " << pp << "\n";
        outf << "\tvertices remaining: " << dsg.num_vertices() <<
                "\tvertices deleted: " << 
                dsg_orig.num_vertices() - dsg.num_vertices() << "\n";
    }

    // spd
    std::time_t start_spd = std::time(nullptr);
    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    std::time_t spd_secs = secs_since(start_spd);
    ttotals.spd += spd_secs;
    outf << "\ttime spd: " << spd_secs << "\n";

    // rgds
    std::time_t start_rgds = std::time(nullptr);
    rgds::result_t res = rgds::rgds(dsg, pre_H,
            std::list< std::set<IVertex> >(), dsg.num_vertices() - pre_D.size(),
            pre_D, spd_ord, ncores);
    std::time_t rgds_secs = secs_since(start_rgds);
    ttotals.rgds += rgds_secs;
    outf << "\ttime rgds: " << rgds_secs << "\n";

    // check result
    if(!res.second) {
        throw std::runtime_error("no rgds despite k = card(V(G))");
    }

    if(!helpers::is_ds(dsg_orig, res.first)) {
        throw std::runtime_error("rgds returned D which is not dom set");
    }

    outf << "\tsize of dominating set: " << res.first.size() << "\n";

    return;
}
