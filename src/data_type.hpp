#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include<iostream>
#include<cstring>
#include<set>
#include<map>
using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
typedef unsigned long long int Stats_t;
typedef unsigned int Params_t;

enum Opcode {
    //Core opcode
    // Core sends read instruction to Bus
    CoreRead,
    // Core sends write invalidate to other cores
    // During write, line is brought to cache using CoreRead and invalidate Req is sent to other cores
    CoreCacheInvalidateReq,
    // When dirty line needs to be evicted, core sends writeBack to Bus
    CoreMemWriteBack,
    //Core sends data to bus data request
    CoreDataResponse,
    //Core sends invalidate acknowledgement after receiving invalidateAck from bus
    CoreCacheInvalidateAck,

    //Bus opcode (Bus to Core)
    //When memory completes write operation, it sends Ack to Core
    BusMemWriteAck,
    // Bus sends cache invalidate request to core to invalidate its copy.
    BusCacheInvalidate,
    // Bus sends invalidate ack to core when all cores having line have invalidated
    // its copy
    BusInvalidateAck,
    // Core receives read data from other core through Bus
    BusDataResponse,
    // Bus is requesting data from a core
    BusReadReq,

    // Memory opcode
    // Memory sends MemWriteAck to bus write operation is completed
    MemWriteAck,
    // Memory sends read data to bus
    MemData,
    // Memory read request
    MemRead,
    // Memory write request
    MemWriteBack
};

enum CacheState {
    MODIFIED,
    OWNED,
    EXCLUSIVE,
    SHARED,
    INVALID, // Initital cache state
    // Line (valid) was in modified state in a core. READ request came up and it was cache miss.
    // MemWriteBack needs to be sent to memory. Core will receive MemWriteAck as response
    // When core receives MemWriteAck, RD_MO_TR_INV transition to RD_INV
    RD_MO_TR_INV,
    // Line (valid) was in Shared/Exclusive state and READ request came up and it was cache miss.
    // No need to send MemWriteBack. Send CoreRead to Bus and meanwhile the line will be in RD_INV state
    RD_INV,
    // Line is not valid and READ request comes up from core. State is changed to RD_INV_TR_SH
    RD_INV_TR_SH,
    // Core wants to WRITE to a line and there is cache HIT and line is clean. Need to send invalidate so that
    // other cores invalidate their copies. When the core receives invalidate Ack from other cores,
    // line transition to MODIFIED state
    WR_TR_MODIFIED,
    // Core wants to WRITE to a line (valid and modified) and there is cache MISS. Cache line needs to evicted and state
    // will be set to WR_TR_INV. MemWriteBack will be sent to Bus
    WR_TR_INV,
    // 1) Core wants to WRITE to a line (valid and clean) and there is cache miss. Just change the state to WR_INV.
    // 2) Core wants to WRITE to a line (invalid). It will be cache miss. Just change the state to WR_INV
    WR_INV
};

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
    ll id;
};

enum Parameters {
    CORE_TO_BUS_BUF_SIZE,
    CORE_TO_BUS_RESP_BUF_SIZE,
    BUS_TO_CORE_BUF_SIZE,
    BUS_TO_MEM_BUF_SIZE,
    MEM_TO_BUS_BUF_SIZE
};

// Not using this enum for now. Might use later to filter out parameters
enum Component {
    CORE,
    BUS,
    MEMORY
};

inline Parameters convertStringToParameter( const std :: string &str) {
    if (str == "CORE_TO_BUS_BUF_SIZE") {
        return CORE_TO_BUS_BUF_SIZE;
    } else if (str == "CORE_TO_BUS_RESP_BUF_SIZE") {
        return CORE_TO_BUS_RESP_BUF_SIZE;
    } else if (str == "BUS_TO_CORE_BUF_SIZE") {
        return BUS_TO_CORE_BUF_SIZE;
    } else if (str == "BUS_TO_MEM_BUF_SIZE") {
        return BUS_TO_MEM_BUF_SIZE;
    } else if (str == "MEM_TO_BUS_BUF_SIZE") {
        return MEM_TO_BUS_BUF_SIZE;
    }
    throw invalid_argument("Invalid parameter string");
}

inline std :: string convertParameterToString(Parameters param) {
    switch (param) {
        case CORE_TO_BUS_BUF_SIZE:
            return "CORE_TO_BUS_BUF_SIZE";
        case CORE_TO_BUS_RESP_BUF_SIZE:
            return "CORE_TO_BUS_RESP_BUF_SIZE";
        case BUS_TO_CORE_BUF_SIZE:
            return "BUS_TO_CORE_BUF_SIZE";
        case BUS_TO_MEM_BUF_SIZE:
            return "BUS_TO_MEM_BUF_SIZE";
        case MEM_TO_BUS_BUF_SIZE:
            return "MEM_TO_BUS_BUF_SIZE";
        default:
            throw invalid_argument("Invalid parameter enum");
    }
}
#endif