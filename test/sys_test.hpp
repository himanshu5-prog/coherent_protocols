#ifndef PROCESSOR_TEST_H
#define PROCESSOR_TEST_H

#include "../src/processor/processor.hpp"

void sys_test(string fileName, bool verbose){
   Processor proc;

   //proc.set_cpu_input_file("input/inputFile1.txt");
   proc.set_debug_mode(verbose);
   //proc.load_cpu_inst_q("input/inputFile1.txt");
   //proc.load_cpu_inst_q("input/simple_read.txt");
   proc.load_cpu_inst_q(fileName);
   //proc.load_cpu_inst_q("input/simple_write.txt");
   proc.run_function();

    
   if (verbose){
      cout << "\nProcessor information:\n";
      proc.printInfo();
      //proc.printPerf();
   }
   

}
#endif