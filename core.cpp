#include "core.hpp"

int Core :: getIndex (ll address){
    return (address & 7);
}

void Core :: run_read (Instruction instr){
    int index;
    ll address;

    address = instr.address;

    index = getIndex (address);
    core_to_bus_tr respTr;

    if ( cache[index].valid){
        if ( cache[index].addr == address){
            //cache hit
            cout << "Address: " << address << " :: cache hit on read operation\n";

        } else {
            //cache miss, need to evict this cache line
            cout << " Address: " << address << " :: cache miss on read operation\n";

            if ( cache[index].dirty){
                //need to perform writeback
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = -1;
                respTr.op = "MemWriteBack";
                respTr.valid = true;

                push_core_to_bus_q (respTr);
            }

            //read operation
            respTr.addr = address;
            respTr.coreID = id;

        }
    }    
}


void Core :: run_function (){
    int index;
    ll address;
    Instruction inst;
    if (instr_q.size() > 0){
        inst = instr_q.front();
    }
}