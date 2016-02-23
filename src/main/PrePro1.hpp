#ifndef PREPRO1_H
#define PREPRO1_H

#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "../types/dstypes.hpp"

/**
 * @brief Apply rule 1 from Albers, Fellow & Niedermeier 2004.
 *
 * Init with DSGraph, then apply(). Pass on PP1.pre_D and PP1.pre_H to
 * rgds(...) and PP2, if applied. Adjust k passed on to rgds() by |pre_D|.
 */
class PrePro1 {
public:
    /** Constructor initiating attributes */
    PrePro1(DSGraph& dsg);

    /** IVertices for which rule 1 had no effect */
    std::set<IVertex> no_effect;

    /** IVertices to be added to D as result of rule 1 */
    std::set<IVertex> pre_D;

    /** IVertices colored H as result of adding verts to pre_D */
    std::set<IVertex> pre_H;

    /**
     * Apply preprocessing rule 1 to each vertex v in dsg:
     *     - if N3(v).empty()
     *       - no_effect.insert(v)
     *       - continue
     *     - remove N3(v) from dsg and pre_H
     *     - remove N2(v) from dsg and pre_H
     *     - H.insert(N(v))
     *     - dsg.remove(v) and pre_H.erase(v)
     */
    void apply();

    /**
     * Calculate N1, N2, N3 for BVertex v.
     * @returns [N1(v), N2(v), N3(v)]
     */ 
    std::vector< std::set<BVertex> > get_n_is(const BVertex v) const;    

private:
    DSGraph& dsg;
};

#endif
