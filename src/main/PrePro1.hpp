#ifndef PREPRO1_H
#define PREPRO1_H

#include <map>
#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "../types/dstypes.hpp"

/**
 * @brief Apply rule 1 from Albers, Fellow & Niedermeier 2004.
 *
 * Init with DSGraph, then apply() to carry out rule 1, modifying dsg.
 * Pass on PP1.pre_D and PP1.pre_H to rgds(...) and PP2, if applied. Adjust
 * k passed on to rgds() by |pre_D|.
 */
class PrePro1 {
public:
    /** Set attributes */
    PrePro1(DSGraph& dsg);

    /** IVertices to be added to D as result of rule 1 */
    std::set<IVertex> pre_D;

    /** IVertices colored H as result of adding verts to pre_D */
    std::set<IVertex> pre_H;

    /** Wrapper for calling init() and finish() successively */
    void run();

    /**
     * Calculate N1, N2, N3 for BVertex v.
     * @returns [N1(v), N2(v), N3(v)]
     */ 
    std::vector< std::set<BVertex> > get_n_is(const BVertex v) const;    

private:
    /** Graph to which apply preprocessing; will be modified */
    DSGraph& dsg;

    /** Map IVertex to its n_is; avoid multiple computations of n_is */
    std::map< IVertex, std::vector< std::set<BVertex> > > m;
    
    /**
     * For each v in V(G):
     * - calculate v's n_is and store them in this->m
     * - if N3(v) != { }, add v to pre_D
     */
    void init();

    /**
     * Clear graph: remove vertices in pre_D (which are still in dsg) 
     * from dsg & pre_H and add their neighbours to pre_H. Call only after
     * init() has been executed.
     */
    void finish();

    /** Remove each v in n_i (that's still in dsg) from pre_H, pre_D and dsg */
    void remove_ni(const std::set<BVertex>& ni);
};

#endif
