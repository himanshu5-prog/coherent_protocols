#ifndef SYS_H
#define SYS_H

#include "cpu.hpp"
#include "bus.hpp"
#include "mem.hpp"

class Processor {
    CPU cpu;
    Bus bus;
    Memory mem;

    public:
        void set_cpu_input_file ( string str);
        void load_cpu_inst_q(string str);
        //int get_size_cpu_inst_q();
        void run_function();
        void printInfo();

        void tr_flow_mem_to_bus ();
        void tr_flow_bus_to_mem ();
        
        void tr_flow_bus_to_core ();
        void tr_flow_core_to_bus();

        bool stop_simulation();
};


#endif