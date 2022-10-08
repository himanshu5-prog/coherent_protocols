#include "core.hpp"

int Core :: getIndex (ll address){
    return (address & 7);
}

void Core :: run_read (Instruction inst ){
    int index;
    ll address;
    core_to_bus_tr respTr;

    address = inst.address;
    index = getIndex (address);

    if ( cache[index].valid & cache[index].transactionCompleted){
        
        if ( cache[index].addr == address){
            //cache hit. Nothing to do
        } else {
            // cache miss. Need to evict the cacheline
            if ( cache[index].dirty){
                //need to writeback to memory
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "MemWriteBack";
                respTr.valid = true;

                // need to wait for ack from memory
                cache[index].transactionCompleted = false;
                cache[index].cacheState = "RD_MO_TR_INV";
                push_core_to_bus_q (respTr);
            }
            // send read message
            respTr.addr = address;
            respTr.coreID = id;
            respTr.data = 0;
            respTr.dest = 0;
            respTr.op = "CoreRead";
            respTr.valid = true;

            cache[index].transactionCompleted = false;
            push_core_to_bus_q ( respTr );
        }

    } else if ( !cache[index].valid & cache[index].transactionCompleted){
        // cache miss and the slot is available. Just send read message
        respTr.addr = address;
        respTr.coreID = id;
        respTr.data = 0;
        respTr.dest = 0;
        respTr.op = "CoreRead";
        respTr.valid = true;

        cache[index].valid = true;
        cache[index].transactionCompleted = false;
        cache[index].cacheState = "RD_INV_TR_SH";
        cache[index].data = 0;
        cache[index].addr = address;
        cache[index].dirty = false;
        cache[index].shared = false;

        push_core_to_bus_q ( respTr );

    }

}

void Core :: run_write ( Instruction inst){
    int index;
    ll address;
    core_to_bus_tr respTr;

    address = inst.address;
    index = getIndex (address);

    if ( cache[index].valid & cache[index].transactionCompleted) {

        if ( cache[index].addr == address){
            //cache hit
            if (cache[index].dirty == false){
                //need to send invalidate signal
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "InvalidateReq";
                respTr.valid = true;

                cache[index].cacheState = "WR_TR_MODIFIED";
                cache[index].transactionCompleted = false;
            } else {
                //cache line is already dirty. Nothing to do
            }
        } else if ( (cache[index].addr != address)){
            // Cache miss. cache line needs to be evicted
            //send memWriteBack message if cache line is dirty
            if (cache[index].dirty){
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "MemWriteBack";
                respTr.valid = true;

                cache[index].cacheState = "WR_TR_INV";
                cache[index].transactionCompleted = false;
            }

            // send read message

            respTr.addr = address;
            respTr.coreID = id;
            respTr.data = 0;
            respTr.dest = 0;
            respTr.op = "CoreRead";
            respTr.valid = true;
            cache[index].transactionCompleted = false;
            push_core_to_bus_q ( respTr);

            // send invalidate message
            respTr.addr = address;
            respTr.coreID = id;
            respTr.data = 0;
            respTr.dest = 0;
            respTr.op = "InvalidateReq";
            respTr.valid = true;
            push_core_to_bus_q ( respTr);

        }

    } else if ( !cache[index].valid & cache[index].transactionCompleted ) {
        //cache miss and slot is empty
        //send read message followed by invalidate
        cache[index].addr = address;
        cache[index].cacheState = "WR_INV_SH";
        cache[index].data = 0;
        cache[index].dirty = true;
        cache[index].shared = false;
        cache[index].transactionCompleted = false;
        cache[index].valid = true;

        respTr.addr = address;
        respTr.coreID = id;
        respTr.data = 0;
        respTr.dest = 0;
        respTr.op = "CoreRead";
        respTr.valid = true;

        push_core_to_bus_q (respTr);

        // send invalidate message
        respTr.addr = address;
        respTr.coreID = id;
        respTr.data = 0;
        respTr.dest = 0;
        respTr.op = "InvalidateReq";
        respTr.valid = true;
        push_core_to_bus_q ( respTr);

    }
}

void Core :: run_data_response (bus_to_core_tr reqTr){
    // Some core sent read instruction and got data response

}
void Core :: run_function (){
    int index;
    ll address;
    Instruction inst;
    if (instr_q.size() > 0){
        inst = instr_q.front();

        if (inst.op == "Read"){
            run_read (inst);
        } else if ( inst.op == "Write"){
            run_write (inst);

        }
    }

    if ( bus_core_transaction.valid){
        if (bus_core_transaction.op == "MemWriteAck"){
            // someone must have sent memWriteBack
            run_mem_write_ack ( bus_core_transaction);

        } else if ( bus_core_transaction.op == "CacheInvalidate"){
            run_cache_inv (bus_core_transaction);

        } else if ( bus_core_transaction.op == "InvalidAck"){
            run_inv_ack ( bus_core_transaction);

        } else if ( bus_core_transaction.op == "BusDataResponse"){
            run_data_response (bus_core_transaction);

        } else if ( bus_core_transaction.op == "BusReadReq"){
            run_bus_read_req ( bus_core_transaction);

        }
    }
}