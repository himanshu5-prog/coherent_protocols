#include<iostream>
#include<cstring>
#include "bus.hpp"

using namespace std;

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

void Bus :: printInfo(){
    cout << "Bus information:\n";

    cout << "core_to_bus_q size: " << get_size_core_to_bus_q() << "\n";
    cout << "core_to_bus_resp_q size: " << get_size_core_to_bus_resp_q() << "\n";
    cout << "bus_to_core_q size: " << get_size_bus_to_core_q() << "\n";
    cout << "bus_to_mem_q size: " << get_size_bus_to_mem_q() << "\n";
    cout << "mem_to_bus_q size: " << get_size_mem_to_bus_q() << "\n";
}

void Bus :: printBusInfoAddr ( map <int, string> cacheState){
    map<int, string> :: iterator itr;
    cout << " cache state record:\n";
    for ( itr = cacheState.begin(); itr != cacheState.end(); itr++){
        cout << " core: " << itr->first << " , state: " << itr->second << "\n";
    }
    cout << "---------------------\n";
}

void Bus :: printParams(){
    std :: cout << "Performance parameters for Bus\n";
    perfParam->printParam();
    std :: cout << "-------------------------------------------------------\n";
}

Params_t Bus :: getParameter(Parameters param){
    return perfParam->getParameter(param);
}

void Bus :: incr_clk_cycle (){
    clk_cycle += (ll)1;
}

void Bus :: set_debug_mode ( bool b){
    debugMode = b;
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

    if (debugMode) {
        cout << "Bus :: run_function, core2bus size: "<< get_size_core_to_bus_q ()<< ", mem2bus size: " << get_size_mem_to_bus_q() 
    << ", bus2core: " << get_size_bus_to_core_q() << ", bus2mem: " << get_size_bus_to_mem_q () << "\n";
    }
    core_to_bus_tr frontTr_resp;
    mem_to_bus_tr frontTr_mem;

    if ( mem_to_bus_q.size() > 0){
        frontTr_mem = get_front_mem_to_bus_q();

        if ( frontTr_mem.op == "MemWriteAck"){
            run_mem_ack (frontTr_mem);

        } else if ( frontTr_mem.op == "MemData"){
            run_mem_data ( frontTr_mem);
        } else {
            cout << "Bus:: run_function - mem_to_bus_tr op not found\n";
        }
    }

    if ( core_to_bus_resp_q.size() > 0){
        frontTr_resp = core_to_bus_resp_q.front();

        if ( frontTr_resp.op == "CoreInvalidateAck"){
            run_invalid_ack ( frontTr_resp);

        } else if ( frontTr_resp.op == "CoreDataResponse"){
            run_data_response ( frontTr_resp);
        } else {
            cout << "Bus :: run_function - core_to_bus resp_tr Op not found\n";
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

        if ( frontTr.op == "CoreRead"){
            run_read_req (frontTr);

        } else if ( frontTr.op == "MemWriteBack"){
            run_mem_write_back (frontTr);

        } else if ( frontTr.op == "InvalidateReq"){
            run_inv_req (frontTr);

        } else {
            cout << "Bus :: run_function - core_to_bus_tr Op not found. op: "<< frontTr.op<<"\n";
        }
    }
}

void Bus :: remove_core_busInfo (ll address, int targetCoreID){
    set <int> :: iterator itr;

    assert ( busInfo[address].coreID.find(targetCoreID) != busInfo[address].coreID.end());
    assert ( busInfo[address].cacheState.find(targetCoreID) != busInfo[address].cacheState.end() );

    for ( itr = busInfo[address].coreID.begin(); itr != busInfo[address].coreID.end(); itr++){
        if (*itr == targetCoreID){
            busInfo[address].coreID.erase(itr);
            break;
        }
    }

    busInfo[address].cacheState.erase (targetCoreID);

}

void Bus :: run_read_req ( core_to_bus_tr reqTr){
    
    ll address;
    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    string state;
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
            
            if (debugMode){
                cout << " Bus :: run_read_req - received CoreRead for address: " << address << " and for core " << reqTr.coreID << ", clk_cycle: " << clk_cycle <<"\n";
            }

            if (debugMode) cout << " Bus :: run_Read_req - the address already exist in busInfo and is in valid state\n";

            busInfo[address].core_bus_tr = reqTr;
            // the address is in busInfo. the address is already present in other core. Core in O state will respond.
            bool flag = false;
            //assert(busInfo[address].valid);
            assert( busInfo[address].coreID.size() > 0);

            for (auto& id: busInfo[address].coreID){
                assert(busInfo[address].cacheState.find(id) != busInfo[address].cacheState.end());

                if ( busInfo[address].cacheState[id] == "EXCLUSIVE" || busInfo[address].cacheState[id] == "OWNED" || busInfo[address].cacheState[id] == "MODIFIED"){
                    sourceCore = id;
                    flag = true;

                    if (debugMode) cout << "Bus :: run_read_req: found core: " << id << " in state: " << busInfo[address].cacheState[id]  << "\n";
                    
                    if (busInfo[address].cacheState[id] == "EXCLUSIVE") {
                        state = "SHARED";
                    } else if (busInfo[address].cacheState[id] == "OWNED"){
                        state = "OWNED";
                    } else if ( busInfo[address].cacheState[id] == "MODIFIED"){
                        state = "OWNED";
                    }

                    //if (debugMode) cout << "Bus :: run_read_req: found core: " << id << " in state: " << state << "\n";
                    break;
                }
            }

            //assert(flag);
            if (flag){
                // cache line with exclusive or owned state is found in one of the core
                respOp = "BusReadReq";
                source = "Bus";

                respTr.addr = address;
                respTr.coreID = sourceCore;
                respTr.data = 0;
                respTr.source = source;
                respTr.valid = true;
                respTr.state = state;
                respTr.op = respOp;

                push_bus_to_core_q(respTr);
                busInfo[address].coreID.insert(reqTr.coreID);
                //busInfo[address].cacheState.insert ( pair <int, string> (sourceCore, "TR_SHARED"));
                busInfo[address].cacheState.insert ( pair <int, string> (reqTr.coreID, "TR_SHARED"));
                busInfo[address].valid = false;
                
                if (debugMode) {
                    cout << " Bus :: run_read_req: Sent BusReadReq to the core: " << sourceCore << " clk_cycle: " << clk_cycle << "\n";
                    cout << " busInfo core id: " << reqTr.coreID << " cache state is TR_SHARED\n";
                }
                //pop_core_to_bus_q();
            } else {
                // All cache line are in shared state
                respOp = "MemRead";
                memResp.addr = address;
                memResp.coreID = reqTr.coreID;
                memResp.data = 0;
                memResp.op = respOp;
                memResp.trID = trID;
                memResp.valid = true;

                if (debugMode){
                    cout << "cache state for address: " << address << "\n";
                    printBusInfoAddr (busInfo[address].cacheState);
                }
                push_bus_to_mem_q(memResp);
                busInfo[address].coreID.insert(reqTr.coreID);
                busInfo[address].cacheState.insert ( pair <int, string> (reqTr.coreID, "TR_SHARED"));
                busInfo[address].valid = false;

                trID += 1;
                if (debugMode){
                    cout << " Bus :: run_read_req: All cores are in shared state. Sent mem read. clk_cycle: " << clk_cycle << "\n";
                    cout << " busInfo core id: " << reqTr.coreID << " cache state is TR_SHARED\n";
                }
                //pop_core_to_bus_q();
            }
        }

    } else {
        // the address is not in busInfo. Need to get from memory :(

        if (debugMode){
            cout << " Bus :: run_read_req - received CoreRead for address: " << address << " and for core " << reqTr.coreID <<  " clk_cycle: " << clk_cycle << "\n";
        }
        if (debugMode) {
            cout << " Bus :: run_Read_req - the address does not exist in busInfo and need to access memory\n";
        }

        memResp.addr = address;
        memResp.coreID = reqTr.coreID;
        memResp.data = reqTr.data;
        memResp.op = "MemRead";
        memResp.trID = trID;
        memResp.valid = true;

        // Need to add entry to busInfo with valid set to false   
        Bus_ds b;
        b.valid = false;
        b.state = "Waiting for mem response";
        b.coreID.insert(reqTr.coreID);
        
        b.cacheState.insert (pair<int, string> ( reqTr.coreID, "TR_EXCLUSIVE"));
        b.data = 0;
        b.core_bus_tr = reqTr;
        b.mem_bus_tr.valid = false;

        for ( int i = 0; i < 8; i++)
        {
            b.invAck[i] = false;
            b.invRequest[i] = false;
        }


        busInfo.insert (pair <ll, Bus_ds> (address,b));


        // pushing req to bus_to_mem_q
        
        push_bus_to_mem_q(memResp);

        // the request will stay in core_to_bus queue until the memory supply the data to bus
        //pop_core_to_bus_q();
        trID += (ll) 1;

        if (debugMode) 
            cout << " busInfo core id: " << reqTr.coreID << " cache state is TR_EXCLUSIVE\n";
    }

}

void Bus :: run_mem_write_back ( core_to_bus_tr reqTr){
    // The address was evicted from cache. The cache line is in Modified/ Owned state and hence writeBack
    // is required.
    ll address;
    bus_to_mem_tr memResp;
    address = reqTr.addr;
    // there must be the entry for address in busInfo
    assert ( busInfo.find(address) != busInfo.end());

    string oldState;
    string newState;

    if ( busInfo[address].valid) {
        
        if (debugMode) 
            cout << "Bus :: run_mem_write_back: received memory write-back req for address: " << reqTr.addr << " from core: " << reqTr.coreID << " clk_cycle: " << clk_cycle << "\n";
        memResp.addr = reqTr.addr;
        memResp.coreID = reqTr.coreID;
        memResp.data = reqTr.data;
        memResp.op = "MemWriteBack";
        memResp.trID = trID;
        memResp.valid = true;

        push_bus_to_mem_q(memResp);
        busInfo[address].core_bus_tr = reqTr;
        // need to remove entry for source core from busInfo
        //remove_core_busInfo ( address, reqTr.coreID);

        oldState = busInfo[address].cacheState[reqTr.coreID];
        assert (busInfo[address].cacheState.find(reqTr.coreID) != busInfo[address].cacheState.end());
        busInfo[address].cacheState[reqTr.coreID] = "TR_INVALID";

        newState = "TR_INVALID";

        if (debugMode)
            cout << " Bus :: run_mem_write_back : core id: " << reqTr.coreID << " state changed from " << oldState << " => " << newState << "\n";

        //remove_core_busInfo( reqTr.addr, reqTr.coreID);
        //write back instruction is not removed from queue till data is received from memory (memAck).
        //pop_core_to_bus_q();
        trID += 1;
        busInfo[address].valid = false;
    }

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

    string oldState;
    string newState;

    //if (debugMode) cout << "Bus :: run_inv_req - received invalidation from core id: " << sourceCore << " for address: " << address << " clk_cycle: " << clk_cycle << "\n"; 
    //bus_to_core_tr respTr;
    // address entry must be present in busInfo
    assert ( busInfo.find(address) != busInfo.end());
    assert ( busInfo[address].cacheState.find(sourceCore) != busInfo[address].cacheState.end());
    assert (busInfo[address].coreID.find(sourceCore) != busInfo[address].coreID.end());
    //busInfo[address].core_bus_tr = reqTr;
    //Need to send invalidate message to all cores other than sourceCore
    int coreCount = 0;

    if ( busInfo[address].valid){
        if (debugMode) cout << "Bus :: run_inv_req - received invalidation from core id: " << sourceCore << " for address: " << address << " clk_cycle: " << clk_cycle << "\n"; 
        bus_to_core_tr respTr;
        busInfo[address].core_bus_tr = reqTr;

        
        for (auto& id: busInfo[address].coreID){
            if ( id == sourceCore)
                continue;
            coreCount += 1;
        }

        if ( coreCount == 0){
            // no need to send invalidate. Only source Core is present
            if (debugMode) cout << " Bus :: run_inv_req - only sourceCore is present in busInfo. No need to send invalidation. Popping out core_to_bus_q\n";
            
            oldState = busInfo[address].cacheState[sourceCore];
            newState = "MODIFIED";

            pop_core_to_bus_q();
            busInfo[address].cacheState[sourceCore] = "MODIFIED";

            if (debugMode){
                cout << " Bus :: run_inv_req - core id: " << sourceCore << " changed from " << oldState << " => " << newState << "\n"; 
            }
        } else { 
            for (auto& id: busInfo[address].coreID){
        
                if (id == sourceCore)
                    continue;
        
                busInfo[address].invRequest[id] = true;
                coreCount += 1;
                // need to send invalidation message 
                respTr.addr = address;
                respTr.coreID = id;
                respTr.data = 0;
                respTr.op = "CacheInvalidate";
                respTr.source = to_string(sourceCore);
                respTr.valid = true;
                respTr.state = "INVALIDATE";

                push_bus_to_core_q(respTr);

                if (debugMode) cout << " Bus :: run_inv_req - " << " sent invalidate request to core id: " << id << "\n";
            }
            //No other transaction can be processes till all acks are received.
            busInfo[address].valid = false;
        }
    }

    // Need to wait for inv ack from all cores before the queue can be popped
    //pop_core_to_bus_q();
}

void Bus :: run_data_response ( core_to_bus_tr reqTr){
    // Bus made request to core in owned state
    ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    int dest;
    address = reqTr.addr;
    sourceCore = reqTr.coreID;
    //dest = reqTr.dest;

    bus_to_core_tr respTr;
    bus_to_mem_tr memRespTr;

    // the address must be in busInfo
    assert ( busInfo.find(address) != busInfo.end());
    assert ( busInfo[address].valid == false);
    dest = busInfo[address].core_bus_tr.coreID;

    //busInfo[address].coreID.insert (dest);
    //the source id should be in coreID set and cachestate map data structure
    assert ( busInfo[address].coreID.find(sourceCore) != busInfo[address].coreID.end());
    assert (busInfo[address].cacheState.find(sourceCore) != busInfo[address].cacheState.end());

    assert ( busInfo[address].coreID.find(dest) != busInfo[address].coreID.end());
    assert (busInfo[address].cacheState.find(dest) != busInfo[address].cacheState.end());

    if (!reqTr.valid){
        if (debugMode){
            cout << " Bus :: run_data_response: data response is invalid sent by core: " << reqTr.coreID << " need to send request to memory"<< "\n";
        }

        memRespTr.addr = reqTr.addr;
        memRespTr.coreID = dest;
        memRespTr.data = 0;
        memRespTr.op = "MemRead";
        memRespTr.trID = trID;
        memRespTr.valid = true;

        trID += 1;

        push_bus_to_mem_q(memRespTr);
        pop_core_to_bus_resp_q();
        return;
    }

    if (debugMode)
        cout << " Bus :: run_data_response: Received data response from core: " << sourceCore << " address: " << address << " clk_cycle: " << clk_cycle << "\n";
    //busInfo[address].cacheState.insert ( pair <int, string>(dest, "SHARED"));

    if (busInfo[address].cacheState[sourceCore] == "EXCLUSIVE"){
        busInfo[address].cacheState[sourceCore] = "SHARED";

        if (debugMode) cout << " Bus :: run_data_response: source core id: " << sourceCore << " changed from EXCLUSIVE to SHARED\n";
 
    } else if ( busInfo[address].cacheState[sourceCore] == "MODIFIED"){
        busInfo[address].cacheState[sourceCore] = "OWNED";
        if (debugMode) cout << " Bus :: run_data_response: source core id: " << sourceCore << " changed from MODIFIED to OWNED\n";
    }

    string oldState;
    string newState;

    oldState = busInfo[address].cacheState[dest];
    newState = "SHARED";

    busInfo[address].cacheState[dest] = "SHARED";

    if (debugMode){
        cout << " Bus :: run_data_response: destination core id: " << dest << " cache state changed from " << oldState << " to " << newState << "\n";
    }

    respTr.addr = address;
    respTr.coreID = dest;
    respTr.data = reqTr.data;
    respTr.op = "BusDataResponse";
    respTr.source = to_string(sourceCore);
    respTr.valid = true;
    respTr.state = "SHARED";

    busInfo[address].valid = true;
    push_bus_to_core_q(respTr);
    pop_core_to_bus_q();
    pop_core_to_bus_resp_q();
}

void Bus :: run_invalid_ack (core_to_bus_tr reqTr){
    // The core sent inv ack. The bus info should collect these ack and when all cores have sent
    //acks, then sourceCore should get ack so that it can make its cache line modified.
    ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    int dest;
    
    address = reqTr.addr;
    sourceCore = reqTr.coreID;

    bus_to_core_tr respTr;

    //The cache line must exist in busInfo
    assert ( busInfo.find(address) != busInfo.end());

    if (debugMode){
        cout << " Bus :: run_inv_ack: recieved inv_Ack from core id: " << sourceCore << ", clk_cycle: " << clk_cycle << "\n";
    }
    // Need to send invalidate message to all cores except sourceCore
    // remove the cores from cacheState and coreID set
    assert ( sourceCore < 8);    
    busInfo[address].invAck[sourceCore] = true;

    //remove_core_busInfo ( address, sourceCore);
    int reqCount, ackCount;

    reqCount = 0;
    ackCount = 0;

    // Changing the state of core which sent inv_ack
    assert ( busInfo[address].coreID.find(sourceCore) != busInfo[address].coreID.end() );
    assert ( busInfo[address].cacheState.find(sourceCore) != busInfo[address].cacheState.end());

    string oldState;
    string newState;

    oldState = busInfo[address].cacheState[sourceCore];
    newState = "INVALID";

    busInfo[address].cacheState[sourceCore] = "INVALID";

    if (debugMode) cout << " Bus :: run_inv_ack: core id: " << sourceCore << " cache state changed from " << oldState << " => " << newState << "\n";

    pop_core_to_bus_resp_q();

    for (int i =0; i < 8; i++){
        // Need to check if all the ack are received.
        // If yes, need to send ack to sourceCore
                
        if ( busInfo[address].invRequest[i]){
            // the core should exist in coreID set
            assert ( busInfo[address].coreID.find(i) != busInfo[address].coreID.end());
            reqCount += 1;
        }

        if ( busInfo[address].invAck[i]){
            ackCount += 1;
        }
    }

    if (debugMode) {
        cout << "Bus::run_inv_ack : number of inv req sent: " << reqCount << ", ack received: " << ackCount << "\n";
    }

    if ( ackCount == reqCount){
        // All cores have responded by sending ack
        //Send ack to sourceCore
        //pop_core_to_bus_q();
        if (debugMode){
            cout << " Bus :: run_invalid_ack: received all inv_ack from all cores. Sending ack to core: " << busInfo[address].core_bus_tr.coreID <<
             " clk_cycle: "<< clk_cycle << "\n";
        }
        
        respTr.addr = address;
        respTr.coreID = busInfo[address].core_bus_tr.coreID;
        respTr.data = 0;
        respTr.op = "BusInvalidateAck";
        respTr.source = "Bus";
        respTr.valid = true;
        respTr.state = "MODIFIED";

        for ( int i = 0; i< 8; i++){
            if ( busInfo[address].invAck[i] & busInfo[address].invRequest[i]){
                remove_core_busInfo (address, i);
                busInfo[address].invAck[i] = false;
                busInfo[address].invRequest[i] = false;
            }
        }

        int reqCore =  busInfo[address].core_bus_tr.coreID;
        assert ( busInfo[address].coreID.find ( reqCore ) != busInfo[address].coreID.end());
        assert (busInfo[address].cacheState.find(reqCore) != busInfo[address].cacheState.end());

        //busInfo[address].coreID.insert ( busInfo[address].core_bus_tr.coreID);
        //busInfo[address].cacheState.insert ( pair <ll, string> (busInfo[address].core_bus_tr.coreID, "MODIFIED" ));
        oldState = busInfo[address].cacheState[reqCore];
        newState = "MODIFIED";

        busInfo[address].cacheState[reqCore] = "MODIFIED";

        if (debugMode) cout << "Bus :: run_inv_ack : cache state of core: " << reqCore << " changed from " << oldState <<  " => MODIFIED\n";
        
        assert (busInfo[address].valid == false);
        busInfo[address].valid = true;

        push_bus_to_core_q (respTr);
        pop_core_to_bus_q();
        //pop_core_to_bus_resp_q();
    }

}

void Bus :: run_mem_ack ( mem_to_bus_tr reqTr) {
    ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    int dest;

    address = reqTr.addr;
    sourceCore = reqTr.coreID;
    bus_to_core_tr respTr;

    // there must be an entry for address in busInfo
    assert (busInfo.find(address) != busInfo.end());
    assert ( busInfo[address].core_bus_tr.op == "MemWriteBack");
    assert ( busInfo[address].valid == false);

    assert ( busInfo[address].coreID.find( sourceCore) != busInfo[address].coreID.end());
    assert ( busInfo[address].cacheState.find(sourceCore) != busInfo[address].cacheState.end());

    assert ( busInfo[address].cacheState[sourceCore] == "TR_INVALID");
    busInfo[address].cacheState.erase(sourceCore);
    busInfo[address].coreID.erase (sourceCore);

    respTr.addr = address;
    respTr.coreID = reqTr.coreID;
    respTr.op = "MemWriteAck";
    respTr.data = 0;
    respTr.source = "Memory";
    respTr.valid = true;
    respTr.state = "INVALID";

    push_bus_to_core_q(respTr);
    busInfo[address].valid = true;

    pop_mem_to_bus_q();
    // since mem ack has been sent to core by bus, so core_to_bus_q can be popped
    pop_core_to_bus_q();

}

void Bus :: run_mem_data ( mem_to_bus_tr reqTr) {
     ll address;

    int coreID;
    string respOp;
    ll data;
    string source;
    int sourceCore;
    int dest;

    address = reqTr.addr;
    sourceCore = reqTr.coreID;

    if (debugMode){
        cout <<" Bus :: Received data from memory for address: " << address << " core id: " << sourceCore << " sending data to core. clk_cycle: " << clk_cycle << "\n";
    }
    bus_to_core_tr respTr;

    assert (busInfo.find(address) != busInfo.end());
    assert ( busInfo[address].valid == false);
    assert ( busInfo[address].core_bus_tr.op == "CoreRead");

    assert ( busInfo[address].coreID.find(sourceCore) != busInfo[address].coreID.end());
    assert ( busInfo[address].cacheState.find( sourceCore) != busInfo[address].cacheState.end());

    if ( busInfo[address].cacheState[sourceCore] == "TR_SHARED"){
        busInfo[address].cacheState[sourceCore] = "SHARED";

        if (debugMode) cout << " Bus :: run_mem_data: core id: " << sourceCore << " cache state changed from TR_SHARED to SHARED\n";

    } else if (busInfo[address].cacheState[sourceCore] == "TR_EXCLUSIVE") {
        busInfo[address].cacheState[sourceCore] = "EXCLUSIVE";

        if (debugMode) cout << " Bus :: run_mem_data: core id: " << sourceCore << " cache state changed from TR_EXCLUSIVE to EXCLUSIVE\n";
    }

    respTr.addr = address;
    respTr.coreID = reqTr.coreID;
    respTr.data = reqTr.data;
    respTr.op = "BusDataResponse";
    respTr.source = "Memory";
    respTr.state = busInfo[address].cacheState[sourceCore];

    push_bus_to_core_q(respTr);
    busInfo[address].valid = true;

    pop_mem_to_bus_q();
    pop_core_to_bus_q();
}