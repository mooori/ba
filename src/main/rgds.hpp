#ifndef RGDS_H
#define RGDS_H

#include <list>
#include <set>
#include <utility>

#include "DSGraph.hpp"
#include "FuncIter.hpp"
#include "../types/dstypes.hpp"

/**
 * @brief Implementation of RGDS, SOLVE and some helper functions
 */
namespace rgds {

    typedef std::pair<std::set<IVertex>, bool> result_t;

    result_t no_solution_res();

    result_t rgds(DSGraph DSG, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int k,
            std::set<IVertex> D);

    result_t solve(std::list<DSGraph*>* comps, std::set<IVertex> H,
            std::list< std::set<IVertex> > Fs, unsigned int k,
            std::set<IVertex> VG, std::set<IVertex> D);

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
            std::set<IVertex> D);

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
            std::set<IVertex> D);

    /** translate list of IColors to BColors */
    std::set<BVertex> trans_I2B(const DSGraph& DSG,
            const std::set<IVertex>& il);

}    // namespace

#endif
