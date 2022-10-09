#ifndef CPU_H
#define CPU_H

#include "core.hpp"
#include <fstream>
#include <vector>

class CPU {
    Core core[8];
    queue <core_to_bus_tr> core2bus_q;
    queue <core_to_bus_tr> core2bus_resp_q;
    queue <bus_to_core_tr> bus2core_q;
    queue <Instruction> inst_q;

    bus_to_core_tr bus2core_tr;
    string inputFile;
    public:
        CPU (){
            for (int i=0; i<8; i++){
                core[i].set_id(i);
                core[i].reset_bus_to_core_tr();
            }
        }

        void set_input_file (string str);
        void printInfo();
        void load_inst_q(string fileName);
        
        void push_core_to_bus_q ( core_to_bus_tr reqTr);
        void push_core_to_bus_resp_q ( core_to_bus_tr reqTr);
        void push_bus_to_core_q (bus_to_core_tr reqTr);

        void pop_core_to_bus_q ();
        void pop_core_to_bus_resp_q ();
        void pop_bus_to_core_q ();

        int get_size_inst_q();

        void run_function();

};



#endif