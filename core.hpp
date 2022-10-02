#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <map>
#include <queue>
#include "data_type.hpp"

using namespace std;

class Core {
    map <int, cacheLine> cache;
    int id;
    queue <core_to_bus_tr> q_core_bus;
    queue <Instruction> instr_q;

    public:
        Core (){
            cacheLine c;
            c.addr = 0;
            c.cacheState = "INVALID";
            c.shared = false;
            c.dirty = false;
            c.valid = false;

            for (int i=0; i<8; i++) {
                cache.insert ( pair <int, cacheLine> (i, c));
            }               
        }
        void set_id (int x) { id = x;}
        void run_function ();
        void run_read (Instruction instr);
        void run_write (Instruction instr);
        int getIndex ( ll address);

        void push_core_to_bus_q ( core_to_bus_tr tr);
        void pop_core_to_bus_q ();
        int get_size_core_to_bus_q ();
};

void Core :: push_core_to_bus_q (core_to_bus_tr tr){
    q_core_bus.push(tr);
}

void Core :: pop_core_to_bus_q (){
    q_core_bus.pop();
}

int Core :: get_size_core_to_bus_q (){
    return q_core_bus.size();
}
#endif