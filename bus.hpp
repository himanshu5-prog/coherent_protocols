#ifndef BUS_H
#define BUS_H

#include<iostream>
#include<queue>
#include<map>
#include<cassert>
#include "data_type.hpp"
using namespace std;

class Bus {
    // bus to core buffer
    queue<bus_to_core_tr> bus_to_core_q;

    //core to bus buffer
    queue<core_to_bus_tr> core_to_bus_q;
    
    //bus to memory buffer
    queue<bus_to_mem_tr> bus_to_mem_q;

    //memory to bus buffer
    queue<mem_to_bus_tr> mem_to_bus_q;

    //Bus data structure
    map <ll, Bus_ds> busInfo;

    ll trID;

    public:
        Bus (){
            trID = (ll) 0;
        }
        // Run function
        void run_function();

        void run_read_req(core_to_bus_tr reqTr);
        void run_mem_write_back(core_to_bus_tr reqTr);
        void run_write_req( core_to_bus_tr reqTr);
        void run_inv_req( core_to_bus_tr reqTr);
        void run_data_response (core_to_bus_tr reqTr);
        void run_invalid_ack (core_to_bus_tr reqTr);

        //Functions to push items to queue
        void push_bus_to_core_q( bus_to_core_tr tr);
        void push_core_to_bus_q( core_to_bus_tr tr);
        
        void push_bus_to_mem_q(bus_to_mem_tr tr);
        void push_mem_to_bus_q(mem_to_bus_tr tr);

        //Functions to pop items from queue
        void pop_bus_to_core_q();
        void pop_core_to_bus_q();

        void pop_bus_to_mem_q();
        void pop_mem_to_bus_q();

        //Function to get front item of the queue
        bus_to_core_tr get_front_bus_to_core_q();
        core_to_bus_tr get_front_core_to_bus_q();

        bus_to_mem_tr get_front_bus_to_mem_q();
        mem_to_bus_tr get_front_mem_to_bus_q();   

        //Functions for busInfo
        void add_bus_info ( ll addr, Bus_ds b);
        bool check_address ( ll addr);
        void remove_address (ll addr);
        Bus_ds getInfo( ll addr);
};

//Push function
void Bus :: push_bus_to_core_q( bus_to_core_tr tr){
    bus_to_core_q.push(tr);
}

void Bus :: push_core_to_bus_q( core_to_bus_tr tr){
    core_to_bus_q.push(tr);
}

void Bus :: push_bus_to_mem_q ( bus_to_mem_tr tr){
    bus_to_mem_q.push(tr);
}

void Bus :: push_mem_to_bus_q ( mem_to_bus_tr tr){
    mem_to_bus_q.push(tr);
}

//Pop functions
void Bus :: pop_bus_to_core_q(){
    bus_to_core_q.pop();
}

void Bus :: pop_core_to_bus_q(){
    core_to_bus_q.pop();
}

void Bus :: pop_bus_to_mem_q(){
    bus_to_mem_q.pop();
}

void Bus :: pop_mem_to_bus_q(){
    mem_to_bus_q.pop();
}

//Accessing front item in queue
bus_to_core_tr Bus :: get_front_bus_to_core_q(){
    return bus_to_core_q.front();
}

core_to_bus_tr Bus :: get_front_core_to_bus_q(){
    return core_to_bus_q.front();
}

bus_to_mem_tr Bus :: get_front_bus_to_mem_q(){
    return bus_to_mem_q.front();
}

mem_to_bus_tr Bus :: get_front_mem_to_bus_q(){
    return mem_to_bus_q.front();
}

//Functions for bus info
void Bus :: add_bus_info(ll addr, Bus_ds b){
    busInfo.insert( pair <ll,Bus_ds>(addr,b));
}

bool Bus :: check_address (ll addr){
    if ( busInfo.find(addr) == busInfo.end()) {
        //address not present in the bus info
        return false;
    } else if ( busInfo.find(addr) != busInfo.end()){
        return true;
    }
    return false;
}

Bus_ds Bus :: getInfo( ll addr){
    assert ( check_address (addr));
    return busInfo[addr];
}

void Bus :: remove_address ( ll addr){
    assert( check_address (addr));
    busInfo.erase(addr);
}

void Bus :: run_function(){
    // Search through core_to_bus queue
    /*
        Possible transactions:
        1) Read
        2) MemWriteBack
        3) Write
    */
    if ( core_to_bus_q.size() > 0){
        
        core_to_bus_tr frontTr;
        bus_to_mem_tr memResp;

        ll address;
        int coreID;
        string respOp;
        ll data;
        string source;
        int sourceCore;

        //Response transaction
        bus_to_core_tr respTr;

        //get the transaction from core_to_bus_q (inputQueue)
        frontTr = get_front_core_to_bus_q();

        if ( frontTr.op == "Read"){
            run_read_req (frontTr);

        } else if ( frontTr.op == "Write"){
            run_write_req(frontTr);

        } else if ( frontTr.op == "memWriteBack"){
            run_mem_write_back (frontTr);

        } else if ( frontTr.op == "invalidateReq"){
            run_data_response (frontTr);

        } 
    }
}
#endif