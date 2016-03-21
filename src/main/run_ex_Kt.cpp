#include <ctime>
#include <iostream>
#include <iomanip>
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
 * @param fname name of file to which to append
 */
void run_graph(unsigned int nverts, std::string pp, unsigned int t,
        unsigned int ncores, struct ttotals_t& ttotals, std::string fname);

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
    std::time_t wcol_t;
    unsigned int edges;
    unsigned int n_pp_del;    // number of verts deleted in preprocessing
    unsigned int ds_size_sum;
    unsigned int wcol_sum;
    ttotals_t() : pp(0), spd(0), rgds(0), wcol_t(0), edges(0), n_pp_del(0),
    ds_size_sum(0), wcol_sum(0) { }
};

/**
 * Append to file, doing open - write -close. Do it for each write, since
 * may not have time to wait till whole programm finished.
 */
void appendf(std::string fname, std::string txt) {
    std::ofstream outf;
    outf.open(fname, std::ios::app);
    outf << txt;
    outf.close();
    return;
}

/** Append some string plus string of local time to fname (see std::put_time)*/
void appendf_ltime(std::string fname, std::string txt) {
    std::ofstream outf;
    outf.open(fname, std::ios::app);
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    outf.imbue(std::locale("de_DE.utf8"));
    outf << txt << std::put_time(&tm, "%c %Z") << "\n";
    outf.close();
}

void run(std::string pp, unsigned int t, unsigned int ncores) {
    std::list<unsigned int> nverts{
        10, 20, 30, 40, 50, 80, 100, 500, 1000, 5000, 10000, 20000, 50000,
        80000, 100000  
    };
    unsigned int ngraphs = 100;

    for(std::list<unsigned int>::iterator it = nverts.begin();
            it != nverts.end(); ++it) {
        // open file
        std::string fname = "../stats/nv_" + std::to_string(*it) + "_t_" + std::to_string(t) + "_" + pp;
        
        std::time_t start_nverts = std::time(nullptr);
        struct ttotals_t ttotals;

        for(unsigned int i = 0; i < ngraphs; ++i) {
            std::cout << "Graph " << i << " for nverts = " << *it << "\n";
            appendf(fname, "Graph " + std::to_string(i) + "\n");
            run_graph(*it, pp, t, ncores, ttotals, fname);
        }

        std::ofstream outf;
        outf.open(fname, std::ios::app);
        outf << "\n\n" << std::string(80, '=') << "\n";
        outf << "Total times are:\n";

        std::time_t secs_tot = secs_since(start_nverts);
        outf << "nverts = " << *it << "\ttime: " << secs_tot << ";\t";
        outf << "avg per graph: " << secs_tot/(float)ngraphs << "\n";
        
        outf << "avg edges per graph: " << ttotals.edges/(float)ngraphs << "\n";

        if(pp == "pp1" || pp == "pp2") {
            outf << "pp time: " << ttotals.pp << " for pp = " << pp << ";\t";
            outf << "avg per graph: " << ttotals.pp/(float)ngraphs << "\n";
            outf << "pp total number of verts deleted: " << ttotals.n_pp_del <<
                    "thats " << float(ttotals.n_pp_del)/float(*it * ngraphs)
                    << " of all vertices\n";
        }

        outf << "spd time: " << ttotals.spd << ";\t";
        outf << "avg per graph: " << ttotals.spd/(float)ngraphs << "\n";

        outf << "rgds time: " << ttotals.rgds << ";\t";
        outf << "avg per graph: " << ttotals.rgds/(float)ngraphs << "\n";
        
        outf << "avg dom set size: " << ttotals.ds_size_sum/(float)ngraphs << "\n";

        outf << "avg wcol number = " << ttotals.wcol_sum/(float)ngraphs << "\n";
        outf << "wcol time: " << ttotals.wcol_t << ";\t";
        outf << "avg per graph: " << ttotals.wcol_t/(float)ngraphs << "\n";

        outf.close();
    }
}

void run_graph(unsigned int nverts, std::string pp, unsigned int t,
        unsigned int ncores, struct ttotals_t& ttotals, std::string fname) {
    std::pair<DSGraph, unsigned int> wrapper = rand_g_ex_Kt::create(nverts, t);
    ttotals.edges += wrapper.second;
    DSGraph dsg(wrapper.first);
    DSGraph dsg_orig(dsg);    // for verification, as PrePro changes dsg
    std::set<IVertex> pre_H, pre_D;

    // preprocessing
    std::time_t start_pp = std::time(nullptr);
    if(pp == "pp1") {
        appendf_ltime(fname, "\tStarting pp1 at ");
        PrePro1 pp1(dsg);
        pp1.run();
        pre_H = pp1.pre_H;
        pre_D = pp1.pre_D;
    }

    if(pp == "pp2") {
        appendf_ltime(fname, "\tStarting pp2 at ");
        PrePro2 pp2(dsg);
        pp2.run();
        pre_H = pp2.pre_H;
        pre_D = pp2.pre_D;
    }

    if(pp == "pp1" || pp == "pp2") {    // time & printing for both identical
        std::time_t pp_secs = secs_since(start_pp);
        ttotals.pp += pp_secs;
        ttotals.n_pp_del += nverts - pre_D.size();
        std::ofstream outf;
        outf.open(fname, std::ios::app);
        outf << "\ttime pp: " << pp_secs << " for pp = " << pp << "\n";
        outf << "\tvertices remaining: " << dsg.num_vertices() <<
                "\tvertices deleted: " << 
                dsg_orig.num_vertices() - dsg.num_vertices() << "\n";
        outf.close();
    }

    // spd
    appendf_ltime(fname, "\tStarting spd at ");
    std::time_t start_spd = std::time(nullptr);
    std::vector<IVertex> spd_ord = spd::build_order(dsg);
    std::time_t spd_secs = secs_since(start_spd);
    ttotals.spd += spd_secs;
    appendf(fname, "\ttime spd: " + std::to_string(spd_secs) + "\n");

    // rgds
    appendf_ltime(fname, "\tStarting rgds at ");
    std::time_t start_rgds = std::time(nullptr);
    rgds::result_t res = rgds::rgds(dsg, pre_H,
            std::list< std::set<IVertex> >(),
            dsg_orig.num_vertices() - pre_D.size(), pre_D, spd_ord, ncores);
    std::time_t rgds_secs = secs_since(start_rgds);
    ttotals.rgds += rgds_secs;
    appendf(fname, "\ttime rgds: " + std::to_string(rgds_secs) + "\n");

    // check result
    if(!res.second) {
        throw std::runtime_error("no rgds despite k = card(V(G))");
    }

    if(!helpers::is_ds(dsg_orig, res.first)) {
        throw std::runtime_error("rgds returned D which is not dom set");
    }

    ttotals.ds_size_sum += res.first.size();
    appendf(fname, "\tsize of dominating set: " +
            std::to_string(res.first.size()) + "\n");

    // wcol
    appendf_ltime(fname, "\tStarting wcol at ");
    std::time_t start_wcol = std::time(nullptr);
    int wcol = spd::wcol_r(dsg_orig, (6 * res.first.size()) + 2);
    ttotals.wcol_sum += wcol;
    std::time_t wcol_secs = secs_since(start_wcol);
    ttotals.wcol_t += wcol_secs;
    appendf(fname, "\twcol = " + std::to_string(wcol) +
            "\ttime = " + std::to_string(wcol_secs) + "\n");

    return;
}
