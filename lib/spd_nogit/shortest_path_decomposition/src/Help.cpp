#include <stdlib.h>
#include <iostream>
#include <utility>
#include <string>
//#include <thread>
//#include <chrono>

#include "Common.h"
#include "../include/Help.hpp"


const Vertex_Descr Help::null_vertex = (Vertex_Descr)-1;

const char* Help::chars[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U" };

unsigned int Help::random_int(unsigned int mod) { 
    #ifdef linux
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    return (rand() % mod);
    #endif
    return ((mod+7)*(mod+13)) % mod;
}

void Help::output() {
    std::cout << "" << std::endl;
}

void Help::sleep() {
    //std::this_thread::sleep_for(std::chrono::milliseconds(2));
}
void Help::sleep(int t) {
    //std::this_thread::sleep_for(std::chrono::milliseconds(t));
}
