#ifndef RGDS_H
#define RGDS_H

#include <list>
#include <mutex>
#include <set>
#include <utility>
#include <vector>

#include "DSGraph.hpp"
#include "FuncIter.hpp"
#include "../types/dstypes.hpp"

/**
 * @brief Implementation of RGDS, SOLVE and some helper functions
 */
namespace rgds {

    typedef std::pair<std::set<IVertex>, bool> result_t;

    result_t no_solution_res();

    extern unsigned int nthreads_active;

    extern std::mutex mutex;

    /** RGDS main algorithm */
    result_t rgds(DSGraph DSG, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int k,
            std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
            const unsigned int cores);

    /**
     * Choose RGDS's v to remove from graph as dsg's smallest vertex in ord
     */
    IVertex choose_v_spd(const DSGraph& dsg, const std::vector<IVertex>& ord);

    /** RGDS's solve */
    result_t solve(std::list<DSGraph*>* comps, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int k,
            std::set<IVertex> VG, std::set<IVertex> D,
            const std::vector<IVertex>& spd_ord, const unsigned int ncores);

    /** return true if RGDS's termination cond related to Distance is met */
    bool diams_exceeding(std::list<DSGraph*>* comps, unsigned int k);

    /** return true if RGDS's test for case k == 0 is true */
    bool test_k_eq_0(std::set<IVertex>& VG, std::set<IVertex>& H,
            std::list< std::set<IVertex> >& Fs); 

    /** Get backland of original graph G-v of which comps are components */
    std::set<IVertex> get_backland(std::set<IVertex>& VG,
            std::list<DSGraph*>* comps);

    /** Find min GDS, return ({}, false) if |GDS| > k */
    result_t get_min_gds(DSGraph DSG, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int max_k,
            std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
            const unsigned int ncores);

    /**
     * Big_union over all sets in vector of rgds::result_t's
     */
    std::set<IVertex> big_union_results(std::vector<result_t> res_vec);

    /**
     * Try to find a GDS given function f: \mathcal{F} -> \mathcal{C} \cup {B}
     */
    result_t try_f(FuncIter& f, std::list<DSGraph*>* comps,
            std::set<IVertex> B, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int k,
            std::set<IVertex> D, const std::vector<IVertex>& spd_ord,
            const unsigned int ncores);

    /**
     * Implementing SOLVE's loop over components.
     * @param H H of the graph of which comps_vec are components
     * @param comps_Fs comps_Fs[i] is the list of F-colors assigned to
     *     comps_vec[i]
     * @param res container to store results (to make work_comps compatibel
     *     with multithreadding). Init res.first with rgds::no_solution_res
     *     of size of comps_vec, to avoid expensive resizing. Returns pair of 
     *     vec of responses for each component and bool, which is false if
     *     any component had no solution <= k
     */
    void work_comps(
            std::vector<DSGraph*>& comps_vec, std::set<IVertex> H,
            std::vector< std::list< std::set<IVertex> > > comps_Fs,
            unsigned int k, std::set<IVertex> D,
            const std::vector<IVertex>& spd_ord, const unsigned int ncores,
            std::pair<std::vector<result_t>, bool>& res);

    std::pair<std::vector<result_t>, bool> concurrently_work_comps(
            std::vector<DSGraph*>& comps_vec, std::set<IVertex> H,
            std::vector< std::list< std::set<IVertex> > > comps_Fs,
            unsigned int k, std::set<IVertex> D,
            const std::vector<IVertex>& spd_ord, const unsigned int ncores);

    /** translate list of IColors to BColors */
    std::set<BVertex> trans_I2B(const DSGraph& DSG,
            const std::set<IVertex>& il);

}    // namespace

#endif
