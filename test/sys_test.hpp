#ifndef PROCESSOR_TEST_H
#define PROCESSOR_TEST_H

#include "../processor.hpp"

void sys_test(string fileName){
   Processor proc;

   //proc.set_cpu_input_file("input/inputFile1.txt");
   proc.set_debug_mode(true);
   //proc.load_cpu_inst_q("input/inputFile1.txt");
   //proc.load_cpu_inst_q("input/simple_read.txt");
   proc.load_cpu_inst_q(fileName);
   //proc.load_cpu_inst_q("input/simple_write.txt");
   proc.run_function();
   cout << "\nProcessor information:\n";
   proc.printInfo();

}
#endif