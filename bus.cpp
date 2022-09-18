#include<iostream>
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
                -> Exist in busInfo : The data is present in other core in E/S state. Send 'busRead' request to core (O state) having the data.
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
            respOp = "BusRead";
            source = "Bus";

            respTr.addr = address;
            respTr.coreID = sourceCore;
            respTr.data = 0;
            respTr.source = source;
            respTr.valid = true;

            push_bus_to_core_q(respTr);
            pop_core_to_bus_q();
        }

    } else {
        // the address is not in busInfo. Need to get from memory :(
        memResp.addr = address;
        memResp.coreID = reqTr.coreID;
        memResp.data = reqTr.data;
        memResp.op = "Read";
        memResp.trID = trID;
        memResp.valid = true;

        // Need to add entry to busInfo with valid set to false   
        Bus_ds b;
        b.valid = false;
        b.state = "Waiting for mem response";
        b.coreID.insert(reqTr.coreID);
        b.cacheState.insert (pair<int, string> ( reqTr.coreID, "Exclusive"));

        // pushing req to bus_to_mem_q
        push_bus_to_mem_q(memResp);

        // the request will stay in core_to_bus queue until the memory supply the data to bus
        //pop_core_to_bus_q();
        trID += (ll) 1;
    }

}
