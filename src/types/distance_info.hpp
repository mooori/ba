#ifndef DISTANCE_INFO_H
#define DISTANCE_INFO_H

#include "dstypes.hpp"

/** 
 * @brief Struct to store a graph's distance info.
 */

typedef struct distance_info {
    int radius;
    int diameter;

    distance_info() : 
            radius((std::numeric_limits<int>::max)()),
            diameter(0)
            { }
} distance_info;

#endif
