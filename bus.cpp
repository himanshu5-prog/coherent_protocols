#include<iostream>
#include<cstring>
#include "bus.hpp"

using namespace std;

void Bus :: run_read_req ( core_to_bus_tr reqTr){
    
    ll address;
    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;

    //Response transaction
    bus_to_core_tr respTr;
    bus_to_mem_tr memResp;

    /*
        About Read operation: This instruction comes from core when there is read instruction (cahce miss) in CPU

        What to do when there is read operation:
            i) Check if there is anything for that address in busInfo data structure.
                -> Exist in busInfo : The data is present in other core in E/S state. Send 'busRead' request to core (O state) having the data. If 
                    If there is no cache state of O state, then that cahe line must have evicted and now data needs to be extracted from memory
    */

    address = reqTr.addr;

    if (busInfo.find(address) != busInfo.end()){

        if ( busInfo[address].valid){
            // the address is in busInfo. the address is already present in other core. Core in O state will respond.
            bool flag = false;
            //assert(busInfo[address].valid);
            assert( busInfo[address].coreID.size() > 0);

            for (auto& id: busInfo[address].coreID){
                assert(busInfo[address].cacheState.find(id) != busInfo[address].cacheState.end());

                if ( busInfo[address].cacheState[id] == "Exclusive" || busInfo[address].cacheState[id] == "Owner"){
                    sourceCore = id;
                    flag = true;
                    break;
                }
            }

            assert(flag);
            if (flag){
                // cache line with exclusive or owned state is found in one of the core
                respOp = "BusRead";
                source = "Bus";

                respTr.addr = address;
                respTr.coreID = sourceCore;
                respTr.data = 0;
                respTr.source = source;
                respTr.valid = true;

                push_bus_to_core_q(respTr);
                pop_core_to_bus_q();
            } else {
                // All cache line are in shared state
                respOp = "memRead";
                memResp.addr = address;
                memResp.coreID = sourceCore;
                memResp.data = 0;
                memResp.op = respOp;
                memResp.trID = trID;
                memResp.valid = true;

                push_bus_to_mem_q(memResp);

                trID += 1;
                pop_core_to_bus_q();
            }
        }

    } else {
        // the address is not in busInfo. Need to get from memory :(
        memResp.addr = address;
        memResp.coreID = reqTr.coreID;
        memResp.data = reqTr.data;
        memResp.op = "memRead";
        memResp.trID = trID;
        memResp.valid = true;

        // Need to add entry to busInfo with valid set to false   
        Bus_ds b;
        b.valid = false;
        b.state = "Waiting for mem response";
        b.coreID.insert(reqTr.coreID);
        b.cacheState.insert (pair<int, string> ( reqTr.coreID, "Exclusive"));
        b.data = 0;

        busInfo.insert (pair <ll, Bus_ds> (address,b));


        // pushing req to bus_to_mem_q
        push_bus_to_mem_q(memResp);

        // the request will stay in core_to_bus queue until the memory supply the data to bus
        //pop_core_to_bus_q();
        trID += (ll) 1;
    }

}

void Bus :: run_write_req ( core_to_bus_tr reqTr){
    ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    
    //Response transaction
    bus_to_core_tr respTr;
    bus_to_mem_tr memResp;

    // Write request is sent to bus and there is cache miss.
    // If the address exist in busInfo table, then other cores have the data.
    //Things to do:
    // send invalidate to cores having the address. Wait for invAck from all those cores. Once all invAcks are received, send data to requestor core
    address = reqTr.addr;
    sourceCore = reqTr.coreID;
    
    if ( busInfo.find(address) != busInfo.end() ){
        if ( busInfo[address].valid){

            assert( busInfo[address].coreID.size() > 0);

            busInfo[address].valid = false;
            busInfo[address].cacheState.insert(pair<int, string> (sourceCore, "TR_MODIFIED"));

            for (auto& id: busInfo[address].coreID){
                assert (id < 7);
                busInfo[address].invRequest[id] = true;

                // send inv request to cores
                respTr.addr = address;
                respTr.coreID = id;
                respTr.data = 0;
                respTr.op = "Invalidate";
                respTr.source = "Bus";
                respTr.valid = true;

                push_bus_to_core_q(respTr);
                
            }

            // removing request from core_to_bus queue
            pop_core_to_bus_q();
        }
    } else {
        // the address is not present in busInfo table. Need to get from memory :-/
        memResp.addr = address;
        memResp.coreID = sourceCore;
        memResp.data = 0;
        memResp.op = "memRead";
        memResp.trID = trID;
        memResp.valid = true;

        trID += (ll) 1;

        // Create the entry in busInfo with valid 0
        Bus_ds b;
        b.valid = false;
        b.cacheState.insert( pair <ll, string> ( sourceCore, "TR_MODIFIED"));
        b.data = 0;
        b.state = "TR_MODIFIED_MEM"; // state of busInfo entry
        b.coreID.insert(sourceCore);

        //pushing the request to bus_to_mem_q
        push_bus_to_mem_q(memResp);        
    }
}

void Bus :: run_mem_write_back ( core_to_bus_tr reqTr){
    // The address was evicted from cache. The cache line is in Modified/ Owned state and hence writeBack
    // is required.

    bus_to_mem_tr memResp;

    memResp.addr = reqTr.addr;
    memResp.coreID = reqTr.coreID;
    memResp.data = reqTr.data;
    memResp.op = "memWrite";
    memResp.trID = trID;
    memResp.valid = true;

    push_bus_to_mem_q(memResp);
    //write back instruction is not removed from queue till data is received from memory.
   // pop_core_to_bus_q();
    trID += 1;

}

void Bus :: run_inv_req ( core_to_bus_tr reqTr){
    //There was write instruction in a core and it was cache hit. cache line state: shared
    // There should be an entry for the address in bus
    ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;

    address = reqTr.addr;
    sourceCore = reqTr.coreID;

    bus_to_core_tr respTr;
    // address entry must be present in busInfo
    assert ( busInfo.find(address) != busInfo.end());

    //Need to send invalidate message to all cores other than sourceCore

    for (auto& id: busInfo[address].coreID){
        
        if (id == sourceCore)
            continue;
        
        busInfo[address].invRequest[id] = true;

        // ned to send invalidation message 
        respTr.addr = address;
        respTr.coreID = id;
        respTr.data = 0;
        respTr.op = "cacheInvalidate";
        respTr.source = to_string(sourceCore);
        respTr.valid = true;

        push_bus_to_core_q(respTr);

    }

    pop_core_to_bus_q();
}