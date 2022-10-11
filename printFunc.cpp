#include <iostream>
#include <map>
#include "data_type.hpp"

using namespace std;

void print_instruction (Instruction inst){
    cout <<"--Instruction-------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "address: " << inst.address << " | coreID: " << inst.coreID << " | data: " << inst.data << " | op: " << inst.op << " | valid: " << inst.valid << "\n";
    cout <<"--------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

void print_mem_to_bus_tr ( mem_to_bus_tr tr){
    cout << "----Memory to bus transaction-------------\n";
    cout << "address: " << tr.addr << "\n";
    cout << "coreID: " << tr.coreID << "\n";
    cout << "data: " << tr.data << "\n";
    cout << "op: " << tr.op << "\n";
    cout << "trID: " << tr.trID << "\n";
    cout << "valid: " << tr.valid << "\n";
    cout << "-------------------------------------------\n";
}

void print_bus_to_mem_tr ( bus_to_mem_tr tr){
    cout << "----Bus to Memory transaction---------------\n";
    cout << "address: " << tr.addr << "\n";
    cout << "coreID: " << tr.coreID << "\n";
    cout << "data: " << tr.data << "\n";
    cout << "op: " << tr.op << "\n";
    cout << "trID: " << tr.trID << "\n";
    cout << "valid: " << tr.valid << "\n";
    cout << "---------------------------------------------\n";
}

void print_core_to_bus_tr ( core_to_bus_tr tr){
    cout << "---Core to bus transaction-------------------\n";
    cout << "address: " << tr.addr << "\n";
    cout << "coreID: " << tr.coreID << "\n";
    cout << "Data: " << tr.data << "\n";
    cout << "Dest: " << tr.dest << "\n";
    cout << "Op: " << tr.op << "\n";
    cout << "----------------------------------------------\n";
}

void print_bus_to_core_tr ( bus_to_core_tr tr){
    cout << "----- Bus to core transaction-------------------\n";
    cout << "addr: " << tr.addr << "\n";
    cout << "coreID: " << tr.coreID << "\n";
    cout << "data: " << tr.data << "\n";
    cout << "op: " << tr.op << "\n";
    cout << "source: " << tr.source << "\n";
    cout << "state: " << tr.state << "\n";
    cout << "valid: " << tr.valid << "\n";
    cout << "--------------------------------------------------\n";
}

void print_cache_line (cacheLine c){
    cout << "----Cache line-----------------------------------\n";
    cout << "" << c.addr << "\n";
    cout << "" << c.cacheState << "\n";
    cout << "" << c.data << "\n";
    cout << "" << c.dirty << "\n";
    cout << "" << c.shared << "\n";
    cout << "" << c.transactionCompleted << "\n";
    cout << "" << c.valid << "\n";
    cout <<"---------------------------------------------------\n";
}