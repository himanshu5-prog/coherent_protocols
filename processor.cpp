#include "processor.hpp"

void Processor :: printInfo(){
    cpu.printInfo();
    mem.printMem();
}

void Processor :: set_cpu_input_file (string str){
    cpu.set_input_file (str);
}

void Processor :: run_function(){
    cpu.run_function();
}

/*
int Processor :: get_size_cpu_inst_q(){
    return cpu.get_size_inst_q();
}
*/