#pragma once

#include <stdlib.h>
#include <iostream>
#include <utility>
#include <string>
//#include <thread>
//#include <chrono>

#include "Common.h"

namespace Help {
    extern const Vertex_Descr null_vertex;
    
    extern const char* chars[21];
    
    template<typename T>
    void output(T str) {
        std::cout << str << std::endl;
    }
    
    unsigned int random_int(unsigned int mod);

    void output();
    
    void sleep();
    
    void sleep(int t);
}