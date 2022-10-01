#ifndef MEM_H
#define MEM_H

#include<iostream>
#include<map>
#include "data_type.hpp"
class Memory {
    map <ll, ll> mem;
    mem_to_bus_tr out;
    bool debugMode;

    public: 
        Memory ();
        void initMem();
        void readMem(bus_to_mem_tr in);
        void writeMem ( bus_to_mem_tr in);
        void printMem ();
        void set_debug_mode (bool b){
            debugMode = b;
        }
        mem_to_bus_tr get_mem_tr ();
        void print_out_tr ();
        void run_function (bus_to_mem_tr reqTr);
};

Memory :: Memory (){
    debugMode = false;
    out.addr = 0;
    out.coreID = 0;
    out.data = 0;
    out.op = "NONE";
    out.trID = 0;
    out.valid = false;
}

// Initialisation
void Memory :: initMem(){
    for (int i=0; i < 64; i++){
        mem.insert ( pair <ll,ll> (i,i/2 + 1));
    }
}

//Write operation
void Memory :: writeMem ( bus_to_mem_tr in){
    ll address = in.addr;

    if ( mem.find(address) != mem.end()){
        mem[address] = in.data;
    } else {
        mem.insert ( pair <ll,ll> ( address, in.data));
    }
    out.addr = address;
    out.coreID = in.coreID;
    out.data = in.data;
    out.trID = in.trID;
    out.op = "MemWriteAck";
    out.valid = true;
}

//Read operation
void Memory :: readMem ( bus_to_mem_tr in){
    ll address;

    address = in.addr;

    if ( mem.find(address) != mem.end()){
        // address is present in the memory
        out.addr = address;
        out.data = mem[address];
        out.coreID = in.coreID;
        out.op = "MemData";
        out.trID = in.trID;
    } else {
        // address is not present in memory
        mem.insert( pair <ll,ll> (address, address/2));
        out.addr = address;
        out.data = mem[address];
        out.coreID = in.coreID;
        out.op = "MemData";
        out.trID = in.trID;
    }
    out.valid = true;
}

// printing the memory for debugging
void Memory :: printMem (){
    map <ll,ll> :: iterator itr;

    for ( itr = mem.begin(); itr != mem.end(); itr++){
        cout << " address: " << itr->first << " : " << " data: " << itr->second << "\n";
    }
}

// getting output transaction from memory
mem_to_bus_tr Memory :: get_mem_tr(){
    return out;
}

void Memory :: print_out_tr(){
    cout << " output transaction from memory to bus:\n";
    cout << " operation: " << out.op << "\n";
    cout << " valid: " << out.valid << "\n";
    cout << " address : " << out.addr << "\n";
    cout << " data: " << out.data << "\n";
    cout << " coreID: " << out.coreID << "\n";
    cout << " transaction ID: " << out.trID << "\n";
    cout << "-----------------------------------------\n";
}

void Memory :: run_function ( bus_to_mem_tr reqTr){
    if ( reqTr.op == "memRead"){
        readMem (reqTr);

    } else if ( reqTr.op == "MemWriteBack"){
        writeMem (reqTr);
    }
}
#endif