#include <stdlib.h>
#include <iostream>
#include <string>

#include "../include/Common.h"
#include "../include/Help.hpp"
#include "../include/GraphWrapper.hpp"

namespace SATParser {
    void extractNumbers(std::vector<int> &numbers, std::string line);
    GraphWrapper* createGraph(const std::string fileName);
    void connectVar(GraphWrapper* graph, Vertex_Descr c, int var);
}