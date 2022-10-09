#ifndef MEM_H
#define MEM_H

#include<iostream>
#include<map>
#include "data_type.hpp"
class Memory {
    map <ll, ll> mem;
    mem_to_bus_tr out;
    bool debugMode;

    public: 
        Memory ();
        void initMem();
        void readMem(bus_to_mem_tr in);
        void writeMem ( bus_to_mem_tr in);
        void printMem ();
        void set_debug_mode (bool b){
            debugMode = b;
        }
        mem_to_bus_tr get_mem_tr ();
        void print_out_tr ();
        void run_function (bus_to_mem_tr reqTr);
};


#endif