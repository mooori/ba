#ifndef MINHS_H
#define MINHS_H

#include <list>
#include <set>
#include <utility>
#include <vector>

#include "../types/dstypes.hpp"

/**
 * @brief Find a minimun hitting set.
 *
 * Let the type of vertex v be the set of colors { F \in Colors | v \in F }.
 * If more than one vertex have the same type, except for one representative,
 * all other vertices of that type will be deleted. For hitting set it is not
 * relevant, which vertex of a given type is added.
 */

class MinHS {
public:
    /**
     * Constructor, just setting private attributes
     * @param col_list colors (i.e. sets) which must be hit
     * @param max_k max value of k tried for finding hitting set
     */
    MinHS(const std::list< std::set<IVertex> > col_list, const int max_k);

    /**
     * Getting minimum hitting set. If no HS with |HS| <= max_k is found,
     *     return <{}, false>.
     * @returns pair of:
     *     - first: a minimum hitting set (empty set of no HS with
     *     cardinality < max_k is found).
     *     - second: true if HS with |HS| <= max_k was found
     */
    std::pair<std::set<IVertex>, bool> get();

    /**
     * Build population, which a set containing exactly one vertex for each
     *     type. If there are more than one vertices with same type, the on
     *     with lowest I-id is added to population.
     */
    std::set<IVertex> build_pop();

private:
    /** max k that is tried for finding a HS */
    const unsigned int max_k;

    /** list of colors (i.e. sets) to be hit */
    std::list< std::set<IVertex> > col_list;

    /** col_list as vector, since build_pop extensively uses indexing */
    const std::vector< std::set<IVertex> > col_vec;

    typedef std::map<IVertex, std::set<unsigned int> > map_t;

    /** map vertex v to indices (referring to col_vec) of colors it is in */
    map_t m_v_cidx;

    /**
     * Test if set s hits each color in col_vec (i.e. if s is a hitting set).
     */
    bool each_color_hit(const std::set<IVertex>& s);
    
};

#endif
