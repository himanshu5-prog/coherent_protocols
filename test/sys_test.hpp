#ifndef PROCESSOR_TEST_H
#define PROCESSOR_TEST_H

#include "../src/processor/processor.hpp"

void sys_test(string fileName, bool verbose, std :: unordered_map <Parameters, Params_t> parameters){
   std :: unique_ptr <Processor> proc_ptr = std :: make_unique<Processor>(); 

   //proc.set_cpu_input_file("input/inputFile1.txt");
   proc_ptr->set_debug_mode(verbose);
   proc_ptr->set_parameters(parameters);
   //proc.load_cpu_inst_q("input/inputFile1.txt");
   //proc.load_cpu_inst_q("input/simple_read.txt");
   proc_ptr->load_cpu_inst_q(fileName);
   //proc.load_cpu_inst_q("input/simple_write.txt");
   proc_ptr->run_function_2();

    
   if (verbose){
      cout << "\nProcessor information:\n";
      proc_ptr->printInfo();
      //proc_ptr->printParams();
      cout << "-------------------------------------------------------\n";
      //proc.printPerf();
   }
   

}
#endif