#ifndef SYS_H
#define SYS_H

#include "../cpu/cpu.hpp"
#include "../bus/bus.hpp"
#include "../memory/mem.hpp"
#include "../perf_param/perf_param.hpp"
#include<vector>
#include<memory>

class Processor {
    std :: unique_ptr <CPU> m_cpu_ptr;
    std :: unique_ptr <Bus> m_bus_ptr;
    std :: unique_ptr <Memory> m_mem_ptr;
    Perf_params perf;    

    bool debugMode;

    public:
        Processor();
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