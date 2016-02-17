#ifndef SPD_H
#define SPD_H

#include <vector>

#include "DSGraph.hpp"
#include "../types/dstypes.hpp"

#include "../../../BA-shortest-paths-decomposition/include/GraphWrapper.hpp"

/**
 * @brief Interface to shortest path decomposition: In: DSGraph; Out: spd-order
 */
namespace spd {

    /** Build DSGraph's spd order */
    std::vector<IVertex> build_order(const DSGraph& dsg);

}    //namespace

#endif
