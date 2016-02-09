#ifndef WCOMPS_H
#define WCOMPS_H

#include <list>
#include <queue>

#include "DSGraph.hpp"

/**
 * @brief For a DSGraph DSG, get components of DSG[N_1(W)].
 *
 * To delete pointers, use this->delete_comps_ptrs
 *
 * appproach to get Components in pseudo code:
 * - create list of Components* comps
 * - W_todo = W
 * - nonW = { }
 * - while(W_todo.size() > 0)
 *   - Q.push(*W_todo.begin())
 *   - discovered.insert(*W_todo.begin())
 *   - W_todo.erase(W_todo.begin())
 *   - C = new DSGraph
 *   - comps.push_back(C)
 *   - add Q.front() to C
 *   - while(Q.size() > 0)
 *      - v = Q.pop()
 *      - if v in W: expand_W(C, Q, v)
 *      - else: expand_nonW(C, Q, v); non_W.insert(v)
 *   - add_nonW_nonW_edges(C, nonW)
 *      
 * epxand_W(C, Q, v) 
 * - n1 = neighbours of v in DSG
 * - for u in n1
 *   - if u not in C: add u to C
 *   - if {v, u} not in E_C: add {v, u}
 *   - if not u \in discovered: Q.push(u); discovered.insert(u);
 *   - W_todo.erase(u)
 * 
 * expand_W(C, Q, v)
 * - n1_cap_W = neighbours of v in DSG \cap W
 * - for u in n1_cap_W
 *   - if u not in C: add u to C
 *   - if {v, u} not in E_C: add {v, u} to E_C
 *   - if not u \in discovered: Q.push(u); discovered.insert(u);
 *   - W_todo.erase(u)
 */

class WComps {
public:
    /**
     * Constructor, assigning arguments to private attributes
     * @param DSG DSGraph of which to find components
     * @param W set of BVertices, as defined in RGDS
     */
    WComps(const DSGraph& DSG, const std::set<BVertex>& W);

    /**
     * Get components of DSG[N_1(W)]
     * @returns std::list<DSGraph*>*
     */
    std::list<DSGraph*>* get();

    /**
     * Delete pointers in list of DSGraph*, as returned by this->get()
     */
    static void delete_comps_ptrs(std::list<DSGraph*>* l);

private:
    /** DSGraph of which to find components */
    const DSGraph& DSG;

    /** W set of BVertices */
    std::set<BVertex> W;

    /** Set of BVertices that have already been discovered during get */
    std::set<BVertex> discovered;

    /** Expand component C with all neighbours of v_B, as v_B is in W */
    void expand_W(DSGraph* C, std::queue<BVertex>& Q,
            std::set<BVertex>& W_todo, const BVertex v_B);

    /** Expand component C only with neighbours of v_B which are in W */
    void expand_nonW(DSGraph* C, std::queue<BVertex>& Q,
            std::set<BVertex>& W_todo, const BVertex v_B);

    /** Add edges between nonW-nonW nodes that are in G[V(C)]
     * @param vids nonW IVertices
     */

    void add_nonW_nonW_edges(DSGraph* C, std::set<IVertex>& nonW);
};

#endif
