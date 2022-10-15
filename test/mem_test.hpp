#ifndef MEM_TEST_H
#define MEM_TEST_H

#include "../src/memory/mem.hpp"
//#include "../data_type.hpp"

void memTest (){
    Memory m;
    m.initMem();
    //printing memory content
    m.printMem();
    bus_to_mem_tr tr_item;
    mem_to_bus_tr out_tr_item;

    //read operation
    tr_item.addr = 55;
    tr_item.op = "READ";
    tr_item.coreID = 0;
    tr_item.data = 0; // data doesn't matter
    tr_item.valid = true;
    tr_item.trID = 0;

    m.readMem(tr_item);
    out_tr_item = m.get_mem_tr();

    cout << " output transaction item after read operation\n";
    m.print_out_tr();

    tr_item.data = 234;
    tr_item.trID = 1;
    m.writeMem(tr_item);

    //printing the memory content after write operation
    m.printMem();
    m.print_out_tr();

}
#endif