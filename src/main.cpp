#include <iostream>

#include "utility/Parser.hpp"

int main() {
    Parser p;
    p.hey();
    parsed_graph<int> pg = p.parse_graph_int("g1.txt");
    std::cout << pg.vertices.front() << std::endl;
    pair<int,int> e1 = pg.edges.front();
    std::cout << e1.first << "," << e1.second << endl;
    return 0;
}
