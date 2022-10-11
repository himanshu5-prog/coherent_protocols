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

    // core_to_bus buffer for invAck/coreDataResponse
    queue < core_to_bus_tr > core_to_bus_resp_q;

    //Bus data structure
    map <ll, Bus_ds> busInfo;

    ll trID;
    bool debugMode;

    public:
        Bus (){
            trID = (ll) 0;
            debugMode = false;
        }
        // Run function
        void run_function();
        void remove_core_busInfo(ll address, int targetCore);
        void set_debug_mode ( bool b);

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

        void printInfo();
};


#endif