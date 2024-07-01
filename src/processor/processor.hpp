#ifndef SYS_H
#define SYS_H

#include "../cpu/cpu.hpp"
#include "../bus/bus.hpp"
#include "../memory/mem.hpp"
#include "../perf_param/perf_param.hpp"

class Processor {
    CPU cpu;
    Bus bus;
    Memory mem;
    Perf_params perf;    

    bool debugMode;

    public:
        //void set_cpu_input_file ( string str);
        void load_cpu_inst_q(string str);
        void set_debug_mode ( bool b);
        //int get_size_cpu_inst_q();
        void run_function();
        void run_function_2();
        void printInfo();

        void tr_flow_mem_to_bus ();
        void tr_flow_bus_to_mem ();
        
        void tr_flow_bus_to_core ();
        void tr_flow_core_to_bus();

        bool stop_simulation();
        void printPerf();
};


#endif