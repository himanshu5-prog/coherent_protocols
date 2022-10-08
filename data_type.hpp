#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include<iostream>
#include<cstring>
#include<set>
using namespace std;

typedef long long int ll;

// Input to memory from bus
struct bus_to_mem_tr {
    ll addr;
    ll data;
    string op;
    int coreID;
    ll trID;
    bool valid;
};

//Output from memory to bus
struct mem_to_bus_tr {
    ll addr;
    ll data;
    string op;
    int coreID;
    ll trID;
    bool valid;
};

// Input from cores to bus
struct core_to_bus_tr {
    ll addr;
    string op;
    ll data;
    int coreID;
    bool valid;
    int dest;
};

// Output from bus to core
struct bus_to_core_tr {
    ll addr;
    string op;
    ll data;
    int coreID;
    bool valid;
    string source;
    string state;
};

// Bus DS
struct Bus_ds {
    bool valid;
    set<int> coreID;
    string state;
    map<int, string> cacheState;
    ll data;
    bool invRequest[8];
    bool invAck[8];
    int invSource;
    core_to_bus_tr core_bus_tr;
    mem_to_bus_tr mem_bus_tr;
};

struct cacheLine {
    bool valid;
    bool shared;
    bool dirty;
    ll addr;
    ll data;
    string cacheState;
    bool transactionCompleted;
};

struct Instruction {
    int address;
    string op;
    int coreID;
    bool valid;
    int data;
};

#endif