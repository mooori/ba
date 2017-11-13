#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/Common.h"
#include "../include/Help.hpp"
#include "../include/GraphWrapper.hpp"
#include "../include/SATParser.hpp"

void SATParser::extractNumbers(std::vector<int> &numbers, std::string line) {
	std::string temp = "";

	for (unsigned int i = 0; i <= line.length(); i++) {
		if (line[i] != ' ' && i < line.length()) {
			temp += line[i];
		}
		else {
			int t;
			/*stringstream ss;
			ss << temp;
			ss >> t;*/
            t = std::stoi(temp);
			numbers.push_back(t);
			temp = "";
		}
	}
}

GraphWrapper* SATParser::createGraph(const std::string file_name) {
    GraphWrapper* ret = new GraphWrapper();
    
    //open file
    std::ifstream sat_file(file_name, std::ifstream::in);
    std::vector<int> vars;
    std::vector<int> args;
    std::vector<std::vector<int>> clausels;
    std::string line;
        
    std::getline(sat_file, line);
    
    while (line[0] == 'c') {
        std::getline(sat_file, line);
    }
    
    extractNumbers(args, line.substr(6));
    
    int n_vars = args[0];
    int n_clausels = args[1];
        
    //for every variable
    for (int i = 1; i <= n_vars; i++) {
        Vertex_Descr v0 = (Vertex_Descr) (long) ((i-1)*3);
        Vertex_Descr v1 = (Vertex_Descr) (long) ((i-1)*3 +1);
        Vertex_Descr v2 = (Vertex_Descr) (long) ((i-1)*3 +2);
        ret->addVertices(v0);
        ret->addVertices(v1);
        ret->addVertices(v2);
        ret->addEdges(v0, v1);
        ret->addEdges(v1, v2);
        ret->addEdges(v2, v0);
    }
    
    for (int i = 0; i < n_clausels; i++) {
        Vertex_Descr c = (Vertex_Descr) (long) (n_vars*3+i);
        ret->addVertices(c);
        std::getline(sat_file, line);
        std::vector<int> clausel;
        extractNumbers(clausel, line);
        clausel.pop_back();
        
        for (std::vector<int>::const_iterator it_c = clausel.begin(); it_c != clausel.end(); it_c++) {
            connectVar(ret, c, *it_c);
        }
    }

    Help::output(ret->getSize());
    return ret;
}

void SATParser::connectVar(GraphWrapper* graph, Vertex_Descr c, int var) {
    Vertex_Descr target;
    if (var < 0) {
        var = -var;
        target = (Vertex_Descr) (long) ((var-1)*3 +1);
    }
    else 
        target = (Vertex_Descr) (long) ((var-1)*3);
    graph->addEdges(c, target);
}
