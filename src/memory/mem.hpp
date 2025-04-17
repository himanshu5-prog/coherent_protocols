#ifndef MEM_H
#define MEM_H

#include<iostream>
#include<map>
#include<queue>
#include "../data_type.hpp"
#include "../perf_param/perf_param.hpp"
class Memory {
    map <ll, ll> mem;
    mem_to_bus_tr out;
    queue <bus_to_mem_tr> bus2mem_q;
    queue <mem_to_bus_tr> mem2bus_q;
    PerfParam *perfParam;
    PerfMem perf;

    bool debugMode;
    ll clk_cycle;
    public: 
        Memory ();
        void initMem();
        void readMem(bus_to_mem_tr in);
        void writeMem ( bus_to_mem_tr in);
        void printMem ();
        void set_debug_mode (bool b){
            debugMode = b;
        }

        void set_perf_param (PerfParam *param) { perfParam = param;}
        void printParams();
        Params_t getParameter(Parameters param);
        void push_bus_to_mem_q ( bus_to_mem_tr tr);
        void push_mem_to_bus_q (mem_to_bus_tr tr);

        void pop_bus_to_mem_q();
        void pop_mem_to_bus_q();

        int get_size_bus_to_mem_q();
        int get_size_mem_to_bus_q();

        bus_to_mem_tr get_front_bus_to_mem_q();
        mem_to_bus_tr get_front_mem_to_bus_q();

        mem_to_bus_tr get_mem_tr ();
        void print_out_tr ();
        void run_function ();

        void printInfo();
        void incr_clk_cycle() { clk_cycle += (ll) 1;}
        bool checkMemToBusQ();
        void printPerf();
};

void print_mem_to_bus_tr ( mem_to_bus_tr tr);
void print_bus_to_mem_tr ( bus_to_mem_tr tr);

#endif