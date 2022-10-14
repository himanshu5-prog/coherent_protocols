#ifndef CPU_H
#define CPU_H

#include "core.hpp"
#include <fstream>
#include <vector>
#include <cassert>

class CPU {
    Core core[8];
    bus_to_core_tr bus2core_tr;
    //ll clk_cycle;
    //string inputFile;
    public:
        CPU (){
            for (int i=0; i<8; i++){
                core[i].set_id(i);
                core[i].reset_bus_to_core_tr();
            }
            //clk_cycle = (ll) 0;
        }

        //void set_input_file (string str);
        void printInfo();
        void load_inst_q(string fileName);
        void set_debug_mode (bool b);

        void push_core_to_bus_q ( core_to_bus_tr reqTr, int coreID);
        void push_core_to_bus_resp_q ( core_to_bus_tr reqTr, int coreID);
        void push_bus_to_core_q (bus_to_core_tr reqTr, int coreID);

        void pop_core_to_bus_q (int coreID);
        void pop_core_to_bus_resp_q (int coreID);
        void pop_bus_to_core_q (int coreID);

        core_to_bus_tr get_front_core_to_bus_q(int id);
        core_to_bus_tr get_front_core_to_bus_resp_q(int id);
        bus_to_core_tr get_front_bus_to_core_q(int id);

        int get_size_core_to_bus_q (int id);
        int get_size_core_to_bus_resp_q (int id);
        int get_size_bus_to_core_q (int id);

        int get_size_core_inst_q(int id);
        //int get_size_inst_q();

        void run_function();
        void incr_clk_cycle();
        void printPerf();

};



#endif