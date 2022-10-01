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

    // core_to_bus buffer for invAck
    queue < core_to_bus_tr > core_to_bus_resp_q;

    //Bus data structure
    map <ll, Bus_ds> busInfo;

    ll trID;

    public:
        Bus (){
            trID = (ll) 0;
        }
        // Run function
        void run_function();
        void remove_core_busInfo(ll address, int targetCore);

        void run_read_req(core_to_bus_tr reqTr);
        void run_mem_write_back(core_to_bus_tr reqTr);
        void run_write_req( core_to_bus_tr reqTr);
        void run_inv_req( core_to_bus_tr reqTr);
        void run_data_response (core_to_bus_tr reqTr);
        void run_invalid_ack (core_to_bus_tr reqTr);

        void run_mem_ack (mem_to_bus_tr reqTr);
        void run_mem_data (mem_to_bus_tr reqTr);

        //Functions to push items to queue
        void push_bus_to_core_q( bus_to_core_tr tr);
        void push_core_to_bus_q( core_to_bus_tr tr);
        void push_core_to_bus_resp_q( core_to_bus_tr tr);
        
        void push_bus_to_mem_q(bus_to_mem_tr tr);
        void push_mem_to_bus_q(mem_to_bus_tr tr);

        //Functions to pop items from queue
        void pop_bus_to_core_q();
        void pop_core_to_bus_resp_q();
        void pop_core_to_bus_q();

        void pop_bus_to_mem_q();
        void pop_mem_to_bus_q();

        //Function to get front item of the queue
        bus_to_core_tr get_front_bus_to_core_q();
        core_to_bus_tr get_front_core_to_bus_q();
        core_to_bus_tr get_front_core_to_bus_resp_q();

        bus_to_mem_tr get_front_bus_to_mem_q();
        mem_to_bus_tr get_front_mem_to_bus_q();   

        //Functions for busInfo
        void add_bus_info ( ll addr, Bus_ds b);
        bool check_address ( ll addr);
        void remove_address (ll addr);
        Bus_ds getInfo( ll addr);

        //function to get size of queue

        int get_size_bus_to_core_q ();
        
        int get_size_core_to_bus_q ();
        int get_size_core_to_bus_resp_q ();
        
        int get_size_bus_to_mem_q ();
        int get_size_mem_to_bus_q (); 
};

//Push function
void Bus :: push_bus_to_core_q( bus_to_core_tr tr){
    bus_to_core_q.push(tr);
}

void Bus :: push_core_to_bus_q( core_to_bus_tr tr){
    core_to_bus_q.push(tr);
}

void Bus :: push_core_to_bus_resp_q( core_to_bus_tr tr){
    core_to_bus_resp_q.push(tr);
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

void Bus :: pop_core_to_bus_resp_q(){
    core_to_bus_resp_q.pop();
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

core_to_bus_tr Bus :: get_front_core_to_bus_resp_q(){
    return core_to_bus_resp_q.front();
}

bus_to_mem_tr Bus :: get_front_bus_to_mem_q(){
    return bus_to_mem_q.front();
}

mem_to_bus_tr Bus :: get_front_mem_to_bus_q(){
    return mem_to_bus_q.front();
}

// function to get size of the queue

int Bus :: get_size_bus_to_core_q (){
    return bus_to_core_q.size();
}

int Bus :: get_size_bus_to_mem_q () {
    return bus_to_mem_q.size();
}

int Bus :: get_size_core_to_bus_q (){
    return core_to_bus_q.size();
}

int Bus :: get_size_core_to_bus_resp_q (){
    return core_to_bus_resp_q.size();
}

int Bus :: get_size_mem_to_bus_q () {
    return mem_to_bus_q.size();
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
    core_to_bus_tr frontTr_resp;
    mem_to_bus_tr frontTr_mem;

    if ( mem_to_bus_q.size() > 0){
        frontTr_mem = get_front_mem_to_bus_q();

        if ( frontTr_mem.op == "MemWriteAck"){
            run_mem_ack (frontTr_mem);

        } else if ( frontTr_mem.op == "MemData"){
            run_mem_data ( frontTr_mem);
        }
    }

    if ( core_to_bus_resp_q.size() > 0){
        frontTr_resp = core_to_bus_resp_q.front();

        if ( frontTr_resp.op == "InvalidateAck"){
            run_invalid_ack ( frontTr_resp);

        } else if ( frontTr_resp.op == "DataResponse"){
            run_data_response ( frontTr_resp);
        }
    }

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

        } else if ( frontTr.op == "MemWriteBack"){
            run_mem_write_back (frontTr);

        } else if ( frontTr.op == "InvalidateReq"){
            run_data_response (frontTr);

        } 
    }
}
#endif