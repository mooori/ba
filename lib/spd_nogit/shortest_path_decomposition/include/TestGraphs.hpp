#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <queue>
#include <stack>

#include "../include/Common.h"

namespace TestGraphs {
    void runTestGrid(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode);
    
    void runTestKn(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode);
    
    void runTestSAT(std::string file, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode);
    
    void runTestReg(int max, int k, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode);
    
    void runForestRun(int max, bool test, int debug, bool viz, int choose_arb_mode, int wcol_r, int wcol_mode);
};