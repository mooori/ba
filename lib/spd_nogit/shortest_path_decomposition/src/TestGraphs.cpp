#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "../include/Common.h"
#include "../include/Help.hpp"
#include "../include/GraphWrapper.hpp"
#include "../include/Component.hpp"
#include "../include/SATParser.hpp"
#include "../include/ShortestPathDecomposition.hpp"
#include "../include/TestGraphs.hpp"


void TestGraphs::runTestGrid(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode) {
    ShortestPathDecomposition::ShortestPathDecomposer spd;
    Component* c = new Component();
    GraphWrapper* grid = new GraphWrapper();
    ShortestPathDecomposition::ShortestPathDecomposer::Config config;
    config.test = test;
    config.debug = debug;
    config.viz = viz;
    config.choose_arb_mode = choose_arb_mode;
    
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            int sum = i*max + j;
            grid->addVertices((Vertex_Descr) (long) sum);
            if (j > 0) {
                grid->addEdges((Vertex_Descr) (long) (sum - 1), (Vertex_Descr) (long) sum);
            }
            if (i > 0) {
                grid->addEdges((Vertex_Descr) (long) (sum - max), (Vertex_Descr) (long) (sum));
            }
        }
    }
    
    if (choose_arb_mode == 0)
        c->next_arbitrary_vertex = (Vertex_Descr) (long) 0;
    else if (choose_arb_mode == 1)
        c->next_arbitrary_vertex = (Vertex_Descr) (long) (max + 1);
    
    c->graph = grid;
    c->minor_model = new MinorModel();
    
    spd.init(c);
    spd.decompose(config);
    
    if (wcol_r) {
        Help::output();
        std::cout << "wcol " << wcol_r << ": " << spd.wCol(wcol_mode, wcol_r, debug) << std::endl;
    }
}

void TestGraphs::runTestKn(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode) {
    ShortestPathDecomposition::ShortestPathDecomposer spd;
    Component* c = new Component();
    GraphWrapper* grid = new GraphWrapper();
    ShortestPathDecomposition::ShortestPathDecomposer::Config config;
    config.test = test;
    config.debug = debug;
    config.viz = viz;
    config.choose_arb_mode = choose_arb_mode;
    
    for (int i = 0; i < max; i++) {
        grid->addVertices((Vertex_Descr) (long) i);
        for (int j = i; j >= 0; j--) {
            if (i != j)
                grid->addEdges((Vertex_Descr) (long) i, (Vertex_Descr) (long) j);
        }
    }
    
    c->graph = grid;
    c->minor_model = new MinorModel();
    c->next_arbitrary_vertex = (Vertex_Descr) (long) 0;
    
    spd.init(c);
    spd.decompose(config);
    
    if (wcol_r) {
        Help::output();
        std::cout << "wcol " << wcol_r << ": " << spd.wCol(wcol_mode, wcol_r, debug) << std::endl;
    }
}

void TestGraphs::runTestSAT(std::string file, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode) {
    ShortestPathDecomposition::ShortestPathDecomposer spd;
    Component* c = new Component();
    GraphWrapper* sat = SATParser::createGraph(file);
    ShortestPathDecomposition::ShortestPathDecomposer::Config config;
    config.test = test;
    config.debug = debug;
    config.viz = viz;
    config.choose_arb_mode = choose_arb_mode;
    
    c->graph = sat;
    c->minor_model = new MinorModel();
    c->next_arbitrary_vertex = (Vertex_Descr) 0;
    
    spd.init(c);
    spd.decompose(config);
    
    if (wcol_r) {
        Help::output();
        std::cout << "wcol " << wcol_r << ": " << spd.wCol(wcol_mode, wcol_r, debug) << std::endl;
    }
}

void TestGraphs::runTestReg(int n, int k, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode) {
    ShortestPathDecomposition::ShortestPathDecomposer spd;
    Component* c = new Component();
    GraphWrapper* reg = new GraphWrapper();
    
    //create reg graph here
    std::map<int, int> buckets;
    for (int i = 0; i < n*k; i++) {
        
        int rand1 = Help::random_int(n*k);
        std::cout << "-> " << i << ", " << rand1 << std::endl;
        
        if (i/k == rand1/k) {
            i--;
            continue;
        }
        
        std::map<int, int>::iterator findi = buckets.find(i);
        std::map<int, int>::iterator find1 = buckets.find(rand1);
        
        if (findi != buckets.end()) {
            continue;
        }
        
        if (find1 != buckets.end()) {
            i--;
            continue;
        }
        
        buckets[i] = rand1;
        buckets[rand1] = i;
        std::cout << i << " " << rand1 << std::endl;
        std::cout << rand1 << " " << i << std::endl;
    }
    
    for (int i = 0; i < n; i++) {
        reg->addVertices((Vertex_Descr) (long) i);
    }
    
    while (buckets.size()) {
        std::map<int, int>::iterator first = buckets.begin();
        int source_p = first->first;
        int target_p = first->second;
        int source_v = source_p/k;
        int target_v = target_p/k;
        buckets.erase(source_p);
        buckets.erase(target_p);
        std::cout << "add " << source_v << " - " << target_v << std::endl;
        reg->addEdges((Vertex_Descr) (long) source_v, (Vertex_Descr) (long) target_v);
    }
    
    //reg->print();
        
    ShortestPathDecomposition::ShortestPathDecomposer::Config config;
    config.test = test;
    config.debug = debug;
    config.viz = viz;
    config.choose_arb_mode = choose_arb_mode;
    
    c->graph = reg;
    c->minor_model = new MinorModel();
    c->next_arbitrary_vertex = (Vertex_Descr) 0;
    
    spd.init(c);
    spd.decompose(config);
    
    if (wcol_r) {
        Help::output();
        std::cout << "wcol " << wcol_r << ": " << spd.wCol(wcol_mode, wcol_r, debug) << std::endl;
    }
}

void TestGraphs::runForestRun(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode) {
    GraphWrapper grid;
    
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            int sum = i*max + j;
            grid.addVertices((Vertex_Descr) (long) sum);
            if (j > 0) {
                grid.addEdges((Vertex_Descr) (long) (sum - 1), (Vertex_Descr) (long) sum);
            }
            if (i > 0) {
                grid.addEdges((Vertex_Descr) (long) (sum - max), (Vertex_Descr) (long) (sum));
            }
        }
    }
    
    //type "Graph" is defined in common.h, this type is input for init function
    Graph g = grid.getGraph();
    
    ShortestPathDecomposition::ShortestPathDecomposer spd;
    
    ShortestPathDecomposition::ShortestPathDecomposer::Config config;
    config.test = test;
    config.debug = debug;
    config.viz = viz;
    config.choose_arb_mode = choose_arb_mode;
    
    spd.init(g);
    spd.decompose(config);
    
    std::vector<Vertex_Descr> ret = spd.getOrder();
}
