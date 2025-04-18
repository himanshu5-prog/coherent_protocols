#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <map>
#include <queue>
#include "../data_type/data_type.hpp"
#include "../perf_param/perf_param.hpp"
#include <cassert>
using namespace std;

class Core {
    map <int, cacheLine> cache;
    int id;
    bool debugMode;
    queue <core_to_bus_tr> q_core_bus;
    queue <core_to_bus_tr> q_core2bus_resp;
    queue <Instruction> instr_q;
    bus_to_core_tr bus_core_transaction;
    queue <bus_to_core_tr> q_bus2core;
    PerfParam *perfParam;
    ll clk_cycle;
    PerfStats perf;

    public:
        Core (){
            id = -1;
            cacheLine c;
            c.addr = 0;
            c.cacheState = CacheState::INVALID;
            c.shared = false;
            c.dirty = false;
            c.valid = false;
            c.transactionCompleted = true;

            for (int i=0; i<8; i++) {
                cache.insert ( pair <int, cacheLine> (i, c));
            }
            debugMode = false;
            clk_cycle = (ll)0;       
        }
        void set_id (int x) { id = x;}
        void set_debug_mode( bool b);
        int get_id() {return id;}
        void run_function ();
        void reset_bus_to_core_tr ();
        void set_bus_to_core_tr(  bus_to_core_tr tr);
        void incr_clk_cycle (){ clk_cycle += (ll)1; }
        void set_perf_param (PerfParam *param) { perfParam = param;}
        Params_t getParameter (Parameters param);
        void printParams();

        //from core to bus queue
        void run_read (Instruction instr);
        void run_write (Instruction instr);

        //from bus to core queue
        void run_mem_write_ack ( bus_to_core_tr tr);
        void run_cache_inv ( bus_to_core_tr tr); 
        void run_inv_ack ( bus_to_core_tr tr);
        void run_data_response ( bus_to_core_tr tr);
        void run_bus_read_req ( bus_to_core_tr tr);
        
        int getIndex ( ll address);

        void push_core_to_bus_q ( core_to_bus_tr tr);
        void push_core_to_bus_resp_q ( core_to_bus_tr tr);
        void push_bus_to_core_q ( bus_to_core_tr tr);
        void push_inst_q (Instruction inst);

        void pop_core_to_bus_q ();
        void pop_core_to_bus_resp_q ();
        void pop_bus_to_core_q();
        void pop_inst_q();

        core_to_bus_tr get_front_core_to_bus_q ();
        core_to_bus_tr get_front_core_to_bus_resp_q ();
        bus_to_core_tr get_front_bus_to_core_q();

        int get_size_core_to_bus_q ();
        int get_size_core_to_bus_resp_q ();
        int get_size_bus_to_core_q();
        int get_size_inst_q();

        bool performCheckCoreToBusQ();
        bool performCheckCoreToBusRespQ();

        void printInfo ();
        void printPerf();
};

void printCacheline ( cacheLine c);
void print_instruction(Instruction inst);
void print_core_to_bus_tr(core_to_bus_tr tr);
void print_bus_to_core_tr(bus_to_core_tr tr);
bool is_state_stable (string state);

#endif