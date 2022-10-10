#ifndef PROCESSOR_TEST_H
#define PROCESSOR_TEST_H

#include "../processor.hpp"

void sys_test(){
   Processor proc;

   //proc.set_cpu_input_file("input/inputFile1.txt");
   proc.load_cpu_inst_q("input/inputFile1.txt");
   //proc.printInfo();
   proc.run_function();
   cout << "Processor information:\n";
   proc.printInfo();

}
#endif