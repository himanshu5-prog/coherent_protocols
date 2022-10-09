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
        //int get_size_cpu_inst_q();
        void run_function();
        void printInfo();
};


#endif