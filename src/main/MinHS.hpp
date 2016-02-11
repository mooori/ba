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
 * all other vertices of the type will be deleted. For hitting set it is not
 * relevant, which vertex of a given type is added.
 */

class MinHS {
public:
    MinHS(const std::list< std::set<IVertex> > col_list, const int max_k);

    std::pair<std::set<IVertex>, bool> get();

    std::set<IVertex> build_pop();

private:
    const unsigned int max_k;

    std::list< std::set<IVertex> > col_list;

    const std::vector< std::set<IVertex> > col_vec;

    typedef std::map<IVertex, std::set<unsigned int> > map_t;

    /** map vertex v to indices (referring to col_vec) of colors is in */
    map_t m_v_cidx;

    /**
     * Test if set s hits each color in col_vec (i.e. if s is a hitting set).
     */
    bool each_color_hit(const std::set<IVertex>& s);
    
};

#endif
