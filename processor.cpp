#include "processor.hpp"

void Processor :: printPerf (){
    cout << "Performance metrics for CPU\n";
    cpu.printPerf();
}

void Processor :: printInfo(){
    cpu.printInfo();
    bus.printInfo();
    mem.printInfo();
    //mem.printMem();
}

void Processor :: set_debug_mode (bool b){
    debugMode = b;
    cpu.set_debug_mode(b);
    bus.set_debug_mode(b);
    mem.set_debug_mode(b);
}
//void Processor :: set_cpu_input_file (string str){
 //   cpu.set_input_file (str);
//}

void Processor :: run_function(){
    int clk_cycle;
    cout << "running the processor\n";
    for(clk_cycle = 0; clk_cycle < 50; clk_cycle++){

       // CLK_CYCLE = clk_cycle;

        if (debugMode)
            cout << "clk_cycle: " << clk_cycle << "\n";
        
        tr_flow_bus_to_mem();
        mem.run_function();
        
        tr_flow_bus_to_core();
        bus.run_function();
        tr_flow_mem_to_bus();

        cpu.run_function();
        tr_flow_core_to_bus();
        
        if (stop_simulation()){
            cout << "All queues are empty. stopping the simulation at clk_cycle: " << clk_cycle << "\n";
            break;
        }
        cpu.incr_clk_cycle();
        bus.incr_clk_cycle();
        mem.incr_clk_cycle();
    }
}

void Processor :: load_cpu_inst_q(string fileName){
    cpu.load_inst_q(fileName);
}

void Processor :: tr_flow_bus_to_core(){
    //need to transfer all transactions from bus2core_q in bus to bus2core_q in core

    bus_to_core_tr bus_tr;
    int coreID;
    
    // bus_to_core_q (Bus) => bus_to_core_q (Core)
    while(bus.get_size_bus_to_core_q() > 0){
        bus_tr = bus.get_front_bus_to_core_q();

        coreID = bus_tr.coreID;
        //assert(coreID < 8 && coreID >= 0);
        cpu.push_bus_to_core_q(bus_tr, coreID);

        bus.pop_bus_to_core_q();
    }
}

void Processor :: tr_flow_core_to_bus(){

    core_to_bus_tr tr;

    for (int i=0; i < 8; i++){

        // core_to_bus_q (Core) to core_to_bus (Bus)
        while(cpu.get_size_core_to_bus_q(i) > 0){

            tr = cpu.get_front_core_to_bus_q(i);
            bus.push_core_to_bus_q(tr);

            cpu.pop_core_to_bus_q(i);
        }
        //core_to_bus_resp (Core) to core_to_bus (Bus)
        while ( cpu.get_size_core_to_bus_resp_q(i) > 0){

            tr = cpu.get_front_core_to_bus_resp_q(i);
            bus.push_core_to_bus_resp_q(tr);

            cpu.pop_core_to_bus_resp_q(i);

        }
    }


}

void Processor :: tr_flow_bus_to_mem(){
    bus_to_mem_tr tr;

    // bus_to_mem_q (Bus) to bus_to_mem_q (memory)

    while( bus.get_size_bus_to_mem_q() > 0){
        
        tr = bus.get_front_bus_to_mem_q();
        mem.push_bus_to_mem_q(tr);

        bus.pop_bus_to_mem_q();
    }
}

void Processor :: tr_flow_mem_to_bus(){
    mem_to_bus_tr tr;

    // mem_to_bus_q (memory) to mem_to_bus_q (bus)

    while( mem.get_size_mem_to_bus_q() > 0){

        tr = mem.get_front_mem_to_bus_q();
        bus.push_mem_to_bus_q(tr);

        mem.pop_mem_to_bus_q();
    }

}

bool Processor :: stop_simulation (){


    for (int i=0; i<8; i++){
        if ( cpu.get_size_core_inst_q(i) > 0){
            return false;
        }

        if ( cpu.get_size_bus_to_core_q(i) > 0){
        return false;
        }

        if (cpu.get_size_core_to_bus_q(i) > 0){
            return false;
        }

        if ( cpu.get_size_core_to_bus_resp_q(i) > 0){
            return false;
        }
    }

    if ( bus.get_size_core_to_bus_q() > 0){
        return false;
    }

    if ( bus.get_size_core_to_bus_resp_q() > 0){
        return false;
    }

    if ( bus.get_size_bus_to_core_q() > 0){
        return false;
    }

    if ( bus.get_size_bus_to_mem_q() > 0){
        return false;
    }

    if ( bus.get_size_mem_to_bus_q() > 0){
        return false;
    }

    if ( mem.get_size_bus_to_mem_q() > 0){
        return false;
    }

    if ( mem.get_size_mem_to_bus_q() > 0){
        return false;
    }

    return true;
   
}