#ifndef PREPRO2_H
#define PREPRO2_H

#include <list>
#include <set>
#include <vector>

#include "DSGraph.hpp"
#include "../src/types/dstypes.hpp"

/**
 * @brief Apply rule 2 from Albers, Fellow & Niedermeier 2004
 *
 * Applying after PrePro1:
 * - Merge PrePro2 pre_D and pre_H with those of PrePro1  
 * - In D returned by rgds, check if it contains any gadget vertices from
 *   PrePro2, case 1.1. If yes, check which of {v, w} is not in D and replace
 *   gadget vertex by the missing one from {v, w}.
 */
class PrePro2 {
public:
    /**
     * Set attributes.
     * @param dsg will be modified
     */
    PrePro2(DSGraph& dsg);

    /** IVertices to be added to D as a result of rule 2 */
    std::set<IVertex> pre_D;

    /** IVertices colored H as result adding verts to pre_D */
    std::set<IVertex> pre_H;

    /**
     * If z is a gadget vertex, m_gadget[z] is the set { v, w } for which it
     * serves as a gadget.
     * Each key in m_gadget is a gadgte vertex.
     * If z is not a key in m_gadget, it is not a gadget vertex.
     */
    std::map< IVertex, std::set<IVertex> > m_gadget;

    /** Wrapper for calling init() and finish() successively */
    void run();

    /**
     * Calculate N1, N2, N3 for BVertices {v, w}
     * @returns [N1(v,w), N2(v,w), N3(v,w)]
     */
    std::vector< std::set<BVertex> >
            get_n_is(const std::set<IVertex> set_vw) const;

private:
    /** Graph to which apply preprocessing; will be modified */
    DSGraph& dsg;

    /** List of pairs {v, w} for which rule 2 can be applied */
    std::list< std::set<IVertex> > applicable;

    /** Map { v, w } to its n_is; avoid multiple calculations of n_is */
    std::map< std::set<IVertex>, std::vector< std::set<IVertex> > > m_n_is;

    /**
     * For each {v, w} subset V(G)
     * - calculate n_is
     * - if rule 2 is applicable
     *     - add {v, w} to applicable
     *     - store n_is in m_n_is
     */
    void init();

    /**
     * Call only after init has been executed.
     * For each {v, w} in applicable, apply the corresponding rule to finish.
     * - if v or w has already been removed: continue
     * - if v or w is in map_gadget: continue  
     * - add to pre_D what may be added
     * - anything belonging to N1 that can be removed, remove from
     *      - dsg, pre_H
     * - then remaining neighbours of what's added to pre_D are colored H
     */
    void finish();

    /**
     * True if rule 2 applicable, i.e. N3(v,w) cannot be dominated by a single
     * vertex from N2(v, w) \cup N3(v, w)
     * @param set_vw set of two vertices { v, w }
     */
    bool rule_2_applicable(std::set<IVertex> set_vw);

    /** Return true if {v, w} meet the respective case */
    bool in_case_1(std::set<IVertex> set_vw);
    bool is_case_1_1(std::set<IVertex> set_vw);
    bool is_case_1_2(std::set<IVertex> set_vw);
    bool is_case_1_3(std::set<IVertex> set_vw);
    bool is_case_2(std::set<IVertex> set_vw);

    /** Finish up according to the respective case */
    bool finish_case_1_1(std::set<IVertex> set_vw);
    bool finish_case_1_2(std::set<IVertex> set_vw);
    bool finish_case_1_3(std::set<IVertex> set_vw);
    bool finish_case_2(std::set<IVertex> set_vw);
};

#endif
