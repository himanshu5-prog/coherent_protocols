#include "processor.hpp"

Processor :: Processor(){
    m_cpu_ptr = std :: make_unique<CPU>();
    m_bus_ptr = std :: make_unique<Bus>();
    m_mem_ptr = std :: make_unique<Memory>();
}
void Processor :: printPerf (){
    cout << "Performance metrics for CPU\n";
    m_cpu_ptr->printPerf();
}

void Processor :: printInfo(){
    m_cpu_ptr->printInfo();
    m_bus_ptr->printInfo();
    m_mem_ptr->printInfo();
    //m_mem_ptr->printMem();
}

void Processor :: set_debug_mode (bool b){
    debugMode = b;
    m_cpu_ptr->set_debug_mode(b);
    m_bus_ptr->set_debug_mode(b);
    m_mem_ptr->set_debug_mode(b);
}
//void Processor :: set_cpu_input_file (string str){
 //   m_cpu_ptr->set_input_file (str);
//}

void Processor :: run_function(){
    int clk_cycle;
    cout << "running the processor\n";
    for(clk_cycle = 0; clk_cycle < 50; clk_cycle++){

       // CLK_CYCLE = clk_cycle;

        if (debugMode)
            cout << "clk_cycle: " << clk_cycle << "\n";
        
        tr_flow_bus_to_mem();
        m_mem_ptr->run_function();
        
        tr_flow_bus_to_core();
        m_bus_ptr->run_function();
        tr_flow_mem_to_bus();

        m_cpu_ptr->run_function();
        tr_flow_core_to_bus();
        
        if (stop_simulation()){
            cout << "All queues are empty. stopping the simulation at clk_cycle: " << clk_cycle << "\n";
            break;
        }
        m_cpu_ptr->incr_clk_cycle();
        m_bus_ptr->incr_clk_cycle();
        m_mem_ptr->incr_clk_cycle();
    }
}

void Processor :: run_function_2(){
    int clk_cycle;
    cout << "running the processor (2)\n";

    for(clk_cycle = 0; clk_cycle < 50; clk_cycle++){
        if (debugMode)
            cout << "clk_cycle: " << clk_cycle << "\n";
        
        m_mem_ptr->run_function();

        m_bus_ptr->run_function();
        tr_flow_mem_to_bus();
        tr_flow_bus_to_mem();

        m_cpu_ptr->run_function();
        tr_flow_core_to_bus();
        tr_flow_bus_to_core();

        if (stop_simulation()){
        cout << "All queues are empty. stopping the simulation at clk_cycle: " << clk_cycle << "\n";
        break;
        }
        
        m_cpu_ptr->incr_clk_cycle();
        m_bus_ptr->incr_clk_cycle();
        m_mem_ptr->incr_clk_cycle();


    }

    
}

void Processor :: load_cpu_inst_q(string fileName){
    m_cpu_ptr->load_inst_q(fileName);
}

void Processor :: tr_flow_bus_to_core(){
    //need to transfer all transactions from bus2core_q in bus to bus2core_q in core

    bus_to_core_tr bus_tr;
    int coreID;
    
    // bus_to_core_q (Bus) => bus_to_core_q (Core)
    while(m_bus_ptr->get_size_bus_to_core_q() > 0){
        bus_tr = m_bus_ptr->get_front_bus_to_core_q();

        coreID = bus_tr.coreID;
        //assert(coreID < 8 && coreID >= 0);
        m_cpu_ptr->push_bus_to_core_q(bus_tr, coreID);

        m_bus_ptr->pop_bus_to_core_q();
    }
}

void Processor :: tr_flow_core_to_bus(){

    core_to_bus_tr tr;

    for (int i=0; i < 8; i++){

        // core_to_bus_q (Core) to core_to_bus (Bus)
        while(m_cpu_ptr->get_size_core_to_bus_q(i) > 0){

            tr = m_cpu_ptr->get_front_core_to_bus_q(i);
            m_bus_ptr->push_core_to_bus_q(tr);

            m_cpu_ptr->pop_core_to_bus_q(i);
        }
        //core_to_bus_resp (Core) to core_to_bus (Bus)
        while ( m_cpu_ptr->get_size_core_to_bus_resp_q(i) > 0){

            tr = m_cpu_ptr->get_front_core_to_bus_resp_q(i);
            m_bus_ptr->push_core_to_bus_resp_q(tr);

            m_cpu_ptr->pop_core_to_bus_resp_q(i);

        }
    }


}

void Processor :: tr_flow_bus_to_mem(){
    bus_to_mem_tr tr;

    // bus_to_mem_q (Bus) to bus_to_mem_q (memory)

    while( m_bus_ptr->get_size_bus_to_mem_q() > 0){
        
        tr = m_bus_ptr->get_front_bus_to_mem_q();
        m_mem_ptr->push_bus_to_mem_q(tr);

        m_bus_ptr->pop_bus_to_mem_q();
    }
}

void Processor :: tr_flow_mem_to_bus(){
    mem_to_bus_tr tr;

    // mem_to_bus_q (memory) to mem_to_bus_q (bus)

    while( m_mem_ptr->get_size_mem_to_bus_q() > 0){

        tr = m_mem_ptr->get_front_mem_to_bus_q();
        m_bus_ptr->push_mem_to_bus_q(tr);

        m_mem_ptr->pop_mem_to_bus_q();
    }

}

bool Processor :: stop_simulation (){


    for (int i=0; i<8; i++){
        if ( m_cpu_ptr->get_size_core_inst_q(i) > 0){
            return false;
        }

        if ( m_cpu_ptr->get_size_bus_to_core_q(i) > 0){
        return false;
        }

        if (m_cpu_ptr->get_size_core_to_bus_q(i) > 0){
            return false;
        }

        if ( m_cpu_ptr->get_size_core_to_bus_resp_q(i) > 0){
            return false;
        }
    }

    if ( m_bus_ptr->get_size_core_to_bus_q() > 0){
        return false;
    }

    if ( m_bus_ptr->get_size_core_to_bus_resp_q() > 0){
        return false;
    }

    if ( m_bus_ptr->get_size_bus_to_core_q() > 0){
        return false;
    }

    if ( m_bus_ptr->get_size_bus_to_mem_q() > 0){
        return false;
    }

    if ( m_bus_ptr->get_size_mem_to_bus_q() > 0){
        return false;
    }

    if ( m_mem_ptr->get_size_bus_to_mem_q() > 0){
        return false;
    }

    if ( m_mem_ptr->get_size_mem_to_bus_q() > 0){
        return false;
    }

    return true;
   
}