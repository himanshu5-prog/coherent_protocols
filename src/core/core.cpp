#include "core.hpp"

void Core :: set_debug_mode (bool b){
    debugMode = b;
}
void Core :: push_core_to_bus_q (core_to_bus_tr tr){
    q_core_bus.push(tr);
}

void Core :: push_core_to_bus_resp_q (core_to_bus_tr tr){
    q_core2bus_resp.push(tr);
}

void Core :: push_bus_to_core_q (bus_to_core_tr tr){
    q_bus2core.push(tr);
}

void Core :: push_inst_q (Instruction inst){
    instr_q.push(inst);
}

void Core :: pop_core_to_bus_q (){
    q_core_bus.pop();
}

void Core :: pop_core_to_bus_resp_q (){
    q_core2bus_resp.pop();
}

void Core :: pop_bus_to_core_q (){
    q_bus2core.pop();
}

void Core :: pop_inst_q (){
    instr_q.pop();
}

core_to_bus_tr Core :: get_front_core_to_bus_q(){
    return q_core_bus.front();
}

core_to_bus_tr Core :: get_front_core_to_bus_resp_q(){
    return q_core2bus_resp.front();
}

bus_to_core_tr Core :: get_front_bus_to_core_q(){
    return q_bus2core.front();
}

int Core :: get_size_core_to_bus_q (){
    return q_core_bus.size();
}

int Core :: get_size_core_to_bus_resp_q (){
    return q_core2bus_resp.size();
}

int Core :: get_size_bus_to_core_q (){
    return q_bus2core.size();
}

int Core :: get_size_inst_q(){
    return instr_q.size();
}

void Core :: reset_bus_to_core_tr (){
    bus_core_transaction.valid = false;
}

void Core :: set_bus_to_core_tr ( bus_to_core_tr tr){
    bus_core_transaction = tr;
}

int Core :: getIndex (ll address){
    return (address & 7);
}

void Core :: printPerf(){
    cout << "Perf metrics for core: " << id << "\n";
    cout << "cache hit: " << perf.get_cache_hit() << "\n";
    cout << "cache miss: " << perf.get_cache_miss() << "\n";
    cout << "bus access: " << perf.get_bus_access() << "\n";
    cout << "memory access for read: " << perf.get_mem_access() << "\n";
    cout << "memory write-back: " << perf.get_mem_write_back() << "\n";
    cout << "-------------------------------------------------------\n";
}

void printCacheline ( cacheLine c){
    cout << "----------------------------------------------------------------\n";
    cout << " address: " << c.addr << " valid: " << c.valid << " shared: " << c.shared << " dirty: " << c.dirty << " data: " << c.data << " state: " << c.cacheState
    << " transactionCompleted: " << c.transactionCompleted << "\n";
    cout << "-------------------------------------------------------------------------\n";
}

void Core :: printInfo (){
    cout << "\nInformation for Core id : " << id << "\n";

    cout << "Cache content\n";
    for (int i=0; i<8; i++){
        cout << "index: " << i << " ";
        printCacheline (cache[i]);
    }
    cout << " inst_q size: " << get_size_inst_q() << "\n";
    cout << "core_to_bus_q size: " << get_size_core_to_bus_q() << "\n";
    cout << " core_to_bus_resp_q size: " << get_size_core_to_bus_resp_q() << "\n";
    cout << " bus_to_core_q size: " << get_size_bus_to_core_q() << "\n";
}

void Core :: run_read (Instruction inst ){
    int index;
    ll address;
    core_to_bus_tr respTr;

    address = inst.address;
    index = getIndex (address);

    if ( cache[index].valid & cache[index].transactionCompleted){
        // cache slot is occupied 

        if (debugMode){
            cout << " Core :: run_read : Core id: " << id << " clk_cycle: " << clk_cycle <<endl;
            cout << " current instruction:\n";
            print_instruction(inst);
            cout << "cache address " << address << " | index: " << index << "\n"; 
        }

        if ( cache[index].addr == address){
            //cache hit. Nothing to do
            if (debugMode) cout << "run_read :: Cache hit!\n";
            perf.incr_cache_hit();

        } else {
            // cache miss. Need to evict the cacheline
            perf.incr_cache_miss();

            if (debugMode) cout << "Core :: run_read : Cache miss\n";
            
            if ( cache[index].dirty){
                //need to writeback to memory
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "MemWriteBack";
                respTr.valid = true;

                // need to wait for ack from memory
                cache[index].cacheState = "RD_MO_TR_INV";
                push_core_to_bus_q (respTr);

                if (debugMode) cout << "Core :: run_read:  cache line is valid & dirty and need to be evicted. Sent MemWriteBack\n";
                perf.incr_bus_access();
                //perf.incr_mem_write_back();

            } else {
                cache[index].cacheState = "RD_INV";
                if (debugMode){
                    cout<< " Core :: run_read: cache miss. the slot was occupied by different address.\n"; 
                    cout << " Core :: run_read: cache line is valid and clean and can be replaced silently\n";
                }
                
            }

            // send read message
            respTr.addr = address;
            respTr.coreID = id;
            respTr.data = 0;
            respTr.dest = 0;
            respTr.op = "CoreRead";
            respTr.valid = true;

            perf.incr_bus_access();

            cache[index].transactionCompleted = false;
            push_core_to_bus_q ( respTr );

            if (debugMode) cout << " Core :: run_read: pushed coreRead to core_to_bus_q\n";
        }

    } else if ( !cache[index].valid){
        // cache miss and the slot is available. Just send read message
        perf.incr_cache_miss();

        if (debugMode){
            cout << " Core :: run_read: Core id: " << id << ", clk_cycle: " << clk_cycle <<endl;
            cout << " current instruction:\n";
            print_instruction(inst);
            cout << "cache address " << address << " | index: " << index << "\n"; 
        }
        if (debugMode) cout << "Core :: run_read : Cache miss and slot is empty.\n";

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
        if (debugMode) cout << " Core :: run_read : pushed coreRead to core_to_bus_q\n";
        perf.incr_bus_access();
    }

}

void Core :: run_write ( Instruction inst){
    int index;
    ll address;
    core_to_bus_tr respTr;

    address = inst.address;
    index = getIndex (address);

    if ( cache[index].valid & cache[index].transactionCompleted) {

        if (debugMode){
            cout << " Core :: run_write : Core id: " << id << ", clk_cycle: " << clk_cycle <<  endl;
            cout << " current instruction:\n";
            print_instruction(inst);
            cout << "cache address " << address << " | index: " << index << "\n"; 
        }

        if ( cache[index].addr == address){
            //cache hit
            perf.incr_cache_hit();

            if (cache[index].dirty == false){
                cout << " Core :: read_write : Cacheline is valid. Cache hit. Dirty bit is false. Need to send invalidate the invalidation request\n";
                //need to send invalidate signal
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "InvalidateReq";
                respTr.valid = true;

                push_core_to_bus_q(respTr);
                perf.incr_bus_access();

                cache[index].cacheState = "WR_TR_MODIFIED";
                cache[index].data = inst.data;
                cache[index].transactionCompleted = false;
            } else {

                if ( cache[index].cacheState == "MODIFIED" ){ 
                    if (debugMode) cout << "Core :: read_write : Cacheline is valid. Cache hit. Dirty bit is already set. Got the new data\n";
                    cache[index].data = inst.data;
                    instr_q.pop();
                } else {
                    // the cache is in owned state. need to send invalidate to bus
                    assert (  cache[index].cacheState == "OWNED");
                    respTr.addr = cache[index].addr;
                    respTr.coreID = id;
                    respTr.data = cache[index].data;
                    respTr.dest = 0;
                    respTr.op = "InvalidateReq";
                    respTr.valid = true;

                    push_core_to_bus_q(respTr);
                    perf.incr_bus_access();

                    cache[index].cacheState = "WR_TR_MODIFIED";
                    cache[index].data = inst.data;
                    cache[index].transactionCompleted = false;

                    if (debugMode) cout << "Core :: run_write : Cache line is in owned state. Sent invalidation to bus.\n";
                }
                //cache line is already dirty. Nothing to do
            }
        } else if ( (cache[index].addr != address)){
            // Cache miss. cache line needs to be evicted
            //send memWriteBack message if cache line is dirty
            perf.incr_cache_miss();

            if (cache[index].dirty){
                
                if (debugMode) cout << "Core :: read_write : Cacheline is valid. Cache miss. Current line is dirty. Need to be evicted\n";
                
                respTr.addr = cache[index].addr;
                respTr.coreID = id;
                respTr.data = cache[index].data;
                respTr.dest = 0;
                respTr.op = "MemWriteBack";
                respTr.valid = true;

                push_core_to_bus_q(respTr);
                perf.incr_bus_access();

                cache[index].cacheState = "WR_TR_INV";
            } else {

                if (debugMode) cout << "Core :: read_write : Cacheline is valid. Cache miss. Current line is clean. Need to be evicted\n";
                cache[index].cacheState = "WR_INV";
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

            perf.incr_bus_access();
            
            if (debugMode) cout << " Core :: read_write : Sent coreRead to core_to_bus_q\n";
            // send invalidate message
            respTr.addr = address;
            respTr.coreID = id;
            respTr.data = 0;
            respTr.dest = 0;
            respTr.op = "InvalidateReq";
            respTr.valid = true;

            cache[index].data = inst.data;
            push_core_to_bus_q ( respTr);
            
            if (debugMode) cout << " Core :: read_write : Sent invalidate request to bus\n";
        }

    } else if ( !cache[index].valid & cache[index].transactionCompleted ) {
        //cache miss and slot is empty
        //send read message followed by invalidate
        perf.incr_cache_miss();

        if (debugMode){
            cout << " Core :: run_write: Core id: " << id << ", clk_cycle: " << clk_cycle << endl;
            cout << " current instruction:\n";
            print_instruction(inst);
            cout << "cache address " << address << " | index: " << index << "\n"; 
        }
        if (debugMode) cout << " Core :: read_write : cacheline is empty. cache miss\n";

        cache[index].addr = address;
        cache[index].cacheState = "WR_INV";
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

        perf.incr_bus_access();

        push_core_to_bus_q (respTr);

        if (debugMode) cout <<" Core :: read_write : sent read resp to bus\n";
        // send invalidate message
        respTr.addr = address;
        respTr.coreID = id;
        respTr.data = 0;
        respTr.dest = 0;
        respTr.op = "InvalidateReq";
        respTr.valid = true;

        cache[index].data = inst.data;
        push_core_to_bus_q ( respTr);

    }
}

void Core :: run_data_response (bus_to_core_tr reqTr){
    // Some core sent read instruction and got data response from bus
    ll address;
    int index;

    address = reqTr.addr;

    index = getIndex (address);

    if (debugMode){
        cout << "Core :: run_data_response: Received data response from Bus in core id: " << id << ", clk_cycle: " << clk_cycle <<"\n";
        cout << "address: " << address << " | index: " << index << "\n";
    }

    assert ( cache[index].transactionCompleted == false);

    string oldState;
    string newState;

    if ( cache[index].cacheState == "WR_INV"){
        // Core need to perform write operation 
        if (reqTr.state == "EXCLUSIVE"){

            oldState = cache[index].cacheState;
            newState = "MODIFIED";

            cache[index].transactionCompleted = true;
            cache[index].cacheState = "MODIFIED";
            cache[index].dirty = true;
            cache[index].shared = false;
            cache[index].valid = true;
            cache[index].addr = reqTr.addr;

            if (debugMode){
                cout << "Core :: run_data_response: core id: " << id << " changed state from " << oldState << " to " << newState << "\n";
            }

            cout << "Core :: run_data_response: Completed instruction:\n";
            print_instruction(instr_q.front());
            instr_q.pop();
            perf.incr_mem_access(); // memory is accessed because there was no core having the address
            

        } else {
            cache[index].transactionCompleted = false; // need to do invalidation
            perf.incr_bus_access(); // bus access is added for invalidation which will be sent.
        }

    } else {

        oldState = cache[index].cacheState;
        newState = reqTr.state;
        
        cache[index].transactionCompleted = true; // read transactions complete
        cache[index].data = reqTr.data;
        
        if (debugMode) cout << " Core :: run_data_response: core id: " << id << " changed state from " << oldState << " => " << newState << "\n";

        cout << "Completed instruction:\n";
        print_instruction(instr_q.front());
        instr_q.pop(); 
        cache[index].addr = reqTr.addr;
        cache[index].cacheState = reqTr.state;
        cache[index].dirty = false;
        cache[index].shared = false;
        cache[index].valid = true;    
    }

    pop_bus_to_core_q();
    //cache[index].data = reqTr.data;
    //cache[index].cacheState = reqTr.state;
    //cache[index].dirty = false;
    //cache[index].shared = false;
    //cache[index].valid = true;    
}

void Core :: run_mem_write_ack ( bus_to_core_tr reqTr){
    ll address;
    int index;

    address = reqTr.addr;
    index = getIndex(address);
    assert ( cache[index].transactionCompleted == false);
    
    if (debugMode){
        cout << " Core :: run_mem_write_ack: received mem_Ack from bus for core: " << id << " address: " << address 
        << ", index: " << index << ", clk_cycle: " << clk_cycle << "\n";
    }

    string oldState;
    string newState;

    // The core has sent memWriteBack to bus
    if ( cache[index].cacheState == "WR_TR_INV"){
        
        oldState = "WR_TR_INV";
        newState = "WR_INV";

        if (debugMode){
            cout << "Core :: run_mem_write_ack: core id: " << id << ", state changed from " << oldState << " => " << newState << "\n";
        }
        cache[index].cacheState = "WR_INV";

    } else if ( cache[index].cacheState == "RD_MO_TR_INV"){
        cache[index].cacheState = "RD_INV";
        cache[index].data = 0;

        oldState = "RD_MO_TR_INV";
        newState = "RD_INV";

        if (debugMode){
            cout << "Core :: run_mem_write_ack: core id: " << id << ", state changed from " << oldState << " => " << newState << "\n";
        }
    }

    //cache[index].data = 0;
    cache[index].dirty = false;
    cache[index].shared = false;
    cache[index].valid = false;
    
    //transactionComplete will remain false since there is read/write pending for that cache line    
    pop_bus_to_core_q();
    perf.incr_mem_write_back();
}

void Core :: run_cache_inv (bus_to_core_tr reqTr ) {
    //Bus has sent cacheInvalidate
    // need to send invAck back to bus

    ll address;
    int index;
    
    core_to_bus_tr respTr;

    address = reqTr.addr;
    index = getIndex (address);

    if (debugMode) {
        cout << "Core:: run_cache_inv: received cache invalidation from bus for core id: " << id << " address: " << address
        << " index: " << index << ", clk_cycle: " << clk_cycle << "\n";
        cout << " Changing state from " << cache[index].cacheState << " => " << "INVALID" << "\n";
    }
    //cache[index].data = 0;
    cache[index].cacheState = "INVALID";
    cache[index].dirty = false;
    cache[index].shared = false;
    cache[index].transactionCompleted = true;
    cache[index].valid = false;

    respTr.addr = reqTr.addr;
    respTr.coreID = id;
    respTr.data = 0;
    respTr.dest = 0;
    respTr.op = "CoreInvalidateAck";
    respTr.valid = true;

    push_core_to_bus_resp_q (respTr);
    
    if (debugMode){
        cout << "Core:: run_cache_inv: Sent invalidate ack to bus\n";
    }
    pop_bus_to_core_q();
}

void Core :: run_inv_ack (bus_to_core_tr reqTr){
    // core received invalidate ack from bus.
    // Core can write to cache data now

    ll address;
    ll data_write;
    int index;

    address = reqTr.addr;
    index = getIndex(address);

    if (debugMode){
        cout << " Core :: run_inv_ack: received invalidate ack from bus for core id: " << id << ", address: " << address <<
        ", index: " << index << " . data can be wriiten in cache"<< ", clk_cycle: " << clk_cycle << "\n";
        cout << "Changing state from " << cache[index].cacheState << " => " << reqTr.state << "\n";
    }

    cache[index].cacheState = reqTr.state;
    cache[index].dirty = true;
    cache[index].shared = false;
    cache[index].transactionCompleted = true;
    cache[index].valid = true;

    pop_bus_to_core_q();
    cout << "Completed instruction: \n";
    print_instruction(instr_q.front());
    instr_q.pop(); // instruction is completed now

}

void Core :: run_bus_read_req ( bus_to_core_tr reqTr){
    // Bus sent data request. Core need to send data response

    ll address;
    int index;
    int data;
    core_to_bus_tr respTr;

    address = reqTr.addr;
    index = getIndex(address);
    
    if (debugMode){
        cout << " Core :: run_bus_read_req: received data request from bus in core id: " << id << ", address: " << address <<
         " index: " << index << ", clk_cycle: " << clk_cycle << "\n";
    }
    assert(cache[index].valid);

    data = cache[index].data;
    cache[index].shared = true;
 
    if (cache[index].cacheState == "EXCLUSIVE"){
        cache[index].cacheState = "SHARED";

        if (debugMode) cout << " State changed from EXCLUSIVE => SHARED\n";

    } else if ( cache[index].cacheState == "MODIFIED"){
        cache[index].cacheState = "OWNED";

        if (debugMode) cout << " State changed from MODIFIED => OWNED\n";
    }

    
    respTr.addr = address;
    respTr.coreID = id;
    respTr.data = data;
    respTr.dest = 0;
    respTr.op = "CoreDataResponse";
    respTr.valid = true;

    push_core_to_bus_resp_q (respTr);
    pop_bus_to_core_q();
    

}

void Core :: run_function (){
    int index;
    ll address;
    Instruction inst;

    if (debugMode) cout << " run_function of Core id: " << id << "\n";
    
    if (instr_q.size() > 0){
        inst = instr_q.front();

        if (inst.op == "RD"){
            run_read (inst);
        } else if ( inst.op == "WR"){
            run_write (inst);
        } else {
            cout << "Core id: " << id << " unknown instruction found\n";
        }
    }
    bus_to_core_tr reqTr;
    //reqTr = q_bus2core.front();
    if ( get_size_bus_to_core_q() > 0){

        reqTr = q_bus2core.front();
        
        if (reqTr.op == "MemWriteAck"){
            // someone must have sent memWriteBack
            run_mem_write_ack (reqTr);

        } else if ( reqTr.op == "CacheInvalidate"){
            run_cache_inv (reqTr);

        } else if ( reqTr.op == "BusInvalidateAck"){
            run_inv_ack ( reqTr);

        } else if ( reqTr.op == "BusDataResponse"){
            run_data_response (reqTr);

        } else if ( reqTr.op == "BusReadReq"){
            run_bus_read_req (reqTr);

        } else {
            cout << "Core id: " << id << " unknown Op in bus_to_core tr, op: " << reqTr.op << "\n";
        }
    }
}