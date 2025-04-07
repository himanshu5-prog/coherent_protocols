#include "mem.hpp"

Memory :: Memory (){
    debugMode = false;
    out.addr = 0;
    out.coreID = 0;
    out.data = 0;
    out.op = "NONE";
    out.trID = 0;
    out.valid = false;

    for (int i=0; i < 64; i++){
        mem.insert ( pair <ll,ll> (i,i/2 + 1));
    }

    clk_cycle = (ll)0;
}

void Memory :: push_bus_to_mem_q( bus_to_mem_tr tr){
    bus2mem_q.push(tr);
}

void Memory :: push_mem_to_bus_q( mem_to_bus_tr tr){
    mem2bus_q.push(tr);
}

void Memory :: pop_bus_to_mem_q (){
    bus2mem_q.pop();
}

void Memory :: pop_mem_to_bus_q (){
    mem2bus_q.pop();
}

int Memory :: get_size_bus_to_mem_q(){
    return bus2mem_q.size();
}

int Memory :: get_size_mem_to_bus_q(){
    return mem2bus_q.size();
}

bus_to_mem_tr Memory :: get_front_bus_to_mem_q(){
    return bus2mem_q.front();
}

mem_to_bus_tr Memory :: get_front_mem_to_bus_q(){
    return mem2bus_q.front();
}

void Memory:: printInfo(){

    cout << "\nInformation for Memory:\n";

    cout << "bus_to_mem_q size: " << get_size_bus_to_mem_q() << "\n";
    cout << " mem_to_bus_q size: " << get_size_mem_to_bus_q() << "\n";
}

void Memory :: printParams(){
    cout << "Memory parameters:\n";
    perfParam->printParam();
    cout << "-----------------------------------------\n";
}

Params_t Memory :: getParameter(Parameters param){
    return perfParam->getParameter(param);
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
    mem_to_bus_tr respTr;

    if (debugMode) {
        cout << " Memory :: received memory Write for address: " << address << " and core id: " << in.coreID << ", clk_cycle: " << clk_cycle <<"\n";
    }

    if ( mem.find(address) != mem.end()){
        mem[address] = in.data;
    } else {
        mem.insert ( pair <ll,ll> ( address, in.data));
    }
    respTr.addr = address;
    respTr.coreID = in.coreID;
    respTr.data = in.data;
    respTr.trID = in.trID;
    respTr.op = "MemWriteAck";
    respTr.valid = true;

    push_mem_to_bus_q(respTr);
    pop_bus_to_mem_q();
}

//Read operation
void Memory :: readMem ( bus_to_mem_tr in){
    ll address;

    address = in.addr;
    mem_to_bus_tr respTr;

    if (debugMode) {
        cout << " Memory :: received memory Read for address: " << address << " and core id: " << in.coreID  << ", clk_cycle: " << clk_cycle << "\n";
    }

    if ( mem.find(address) != mem.end()){
        // address is present in the memory
        respTr.addr = address;
        respTr.data = mem[address];
        respTr.coreID = in.coreID;
        respTr.op = "MemData";
        respTr.trID = in.trID;
    } else {
        // address is not present in memory
        mem.insert( pair <ll,ll> (address, address/2));
        respTr.addr = address;
        respTr.data = mem[address];
        respTr.coreID = in.coreID;
        respTr.op = "MemData";
        respTr.trID = in.trID;
    }
    respTr.valid = true;
    push_mem_to_bus_q (respTr);
    pop_bus_to_mem_q();
}

// printing the memory for debugging
void Memory :: printMem (){
    map <ll,ll> :: iterator itr;
    cout << " Memory content:\n";
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

void Memory :: run_function (){
    if (debugMode){
        std :: cout << "Mem :: run_function() :: mem2bus size: " << get_size_mem_to_bus_q() << ", bus2mem size: " << get_size_bus_to_mem_q() << "\n";
    }
    bus_to_mem_tr reqTr;
    //reqTr = bus2mem_q.front();

    //if (debugMode) cout << " Memory run_function()\n";
    if ( get_size_bus_to_mem_q() > 0){
        
        reqTr = bus2mem_q.front();

        if ( reqTr.op == "MemRead"){
            readMem (reqTr);

        } else if ( reqTr.op == "MemWriteBack"){
            writeMem (reqTr);
        } else {
            cout << "Memory: run_function - unknown bus_to_mem_tr found, op: " << reqTr.op <<"\n";
        }
    }
}    