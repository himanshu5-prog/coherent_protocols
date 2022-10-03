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
    bus_to_core_tr bus_core_transaction;

    public:
        Core (){
            cacheLine c;
            c.addr = 0;
            c.cacheState = "INVALID";
            c.shared = false;
            c.dirty = false;
            c.valid = false;
            c.transactionCompleted = true;

            for (int i=0; i<8; i++) {
                cache.insert ( pair <int, cacheLine> (i, c));
            }               
        }
        void set_id (int x) { id = x;}
        void run_function ();
        void reset_bus_to_core_tr ();
        void set_bus_to_core_tr(  bus_to_core_tr tr);

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

void Core :: reset_bus_to_core_tr (){
    bus_core_transaction.valid = false;
}

void Core :: set_bus_to_core_tr ( bus_to_core_tr tr){
    bus_core_transaction = tr;
}

#endif