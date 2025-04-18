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

enum Parameters {
    CORE_TO_BUS_BUF_SIZE,
    CORE_TO_BUS_RESP_BUF_SIZE,
    BUS_TO_CORE_BUF_SIZE,
    BUS_TO_MEM_BUF_SIZE,
    MEM_TO_BUS_BUF_SIZE
};

enum Component {
    CORE_0,
    CORE_1,
    CORE_2,
    CORE_3,
    CORE_4,
    CORE_5,
    CORE_6,
    CORE_7,
    BUS,
    MEMORY
};
// Opcode name starts with its source name.
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
    MemWriteBack,
    NUM_OPCODES
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
    RD_INV_TR_EXCLUSIVE,
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
    Opcode op;
    int coreID;
    ll trID;
    bool valid;
};

// Input from cores to bus
struct core_to_bus_tr {
    ll addr;
    Opcode op;
    ll data;
    int coreID;
    bool valid;
    int dest;
};

// Output from bus to core
struct bus_to_core_tr {
    ll addr;
    Opcode op;
    ll data;
    int coreID;
    bool valid;
    Component source;
    CacheState state;
};

// Bus DS
struct Bus_ds {
    bool valid;
    set<int> coreID;
    CacheState state;
    map<int, CacheState> cacheState;
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
    CacheState cacheState;
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

inline std :: string convertOpcodeToString (Opcode op){
    switch (op){
        case CoreRead:
            return "CoreRead";
        case CoreCacheInvalidateReq:
            return "CoreCacheInvalidateReq";
        case CoreMemWriteBack:
            return "CoreMemWriteBack";
        case CoreDataResponse:
            return "CoreDataResponse";
        case CoreCacheInvalidateAck:
            return "CoreCacheInvalidateAck";
        case BusMemWriteAck:
            return "BusMemWriteAck";
        case BusCacheInvalidate:
            return "BusCacheInvalidate";
        case BusInvalidateAck:
            return "BusInvalidateAck";
        case BusDataResponse:
            return "BusDataResponse";
        case BusReadReq:
            return "BusReadReq";
        case MemWriteAck:
            return "MemWriteAck";
        case MemData:
            return "MemData";
        case MemRead:
            return "MemRead";
        case MemWriteBack:
            return "MemWriteBack";
        case NUM_OPCODES:
            return "NUM_OPCODES";
    }
    throw invalid_argument("Invalid opcode");
}

inline std :: string convertCacheStateToString (CacheState state){
    switch (state){
        case MODIFIED:
            return "MODIFIED";
        case OWNED:
            return "OWNED";
        case EXCLUSIVE:
            return "EXCLUSIVE";
        case SHARED:
            return "SHARED";
        case INVALID:
            return "INVALID";
        case RD_MO_TR_INV:
            return "RD_MO_TR_INV";
        case RD_INV:
            return "RD_INV";
        case RD_INV_TR_SH:
            return "RD_INV_TR_SH";
        case WR_TR_MODIFIED:
            return "WR_TR_MODIFIED";
        case WR_TR_INV:
            return "WR_TR_INV";
        case WR_INV:
            return "WR_INV";
        case RD_INV_TR_EXCLUSIVE:
            return "RD_INV_TR_EXCLUSIVE";
    }
    throw invalid_argument("Invalid cache state");
}

inline std :: string convertComponentToString (Component comp){
    switch (comp){
        case CORE_0:
            return "CORE_0";
        case CORE_1:
            return "CORE_1";
        case CORE_2:
            return "CORE_2";
        case CORE_3:
            return "CORE_3";
        case CORE_4:
            return "CORE_4";
        case CORE_5:
            return "CORE_5";
        case CORE_6:
            return "CORE_6";
        case CORE_7:
            return "CORE_7";
        case BUS:
            return "BUS";
        case MEMORY:
            return "MEMORY";
    }
    throw invalid_argument("Invalid component");
}

inline Component convertCoreIDToComponent (int coreID){
    switch (coreID){
        case 0:
            return CORE_0;
        case 1:
            return CORE_1;
        case 2:
            return CORE_2;
        case 3:
            return CORE_3;
        case 4:
            return CORE_4;
        case 5:
            return CORE_5;
        case 6:
            return CORE_6;
        case 7:
            return CORE_7;
    }
    throw invalid_argument("Invalid core ID");
}
inline int convertComponentToCoreID (Component comp){
    switch (comp){
        case CORE_0:
            return 0;
        case CORE_1:
            return 1;
        case CORE_2:
            return 2;
        case CORE_3:
            return 3;
        case CORE_4:
            return 4;
        case CORE_5:
            return 5;
        case CORE_6:
            return 6;
        case CORE_7:
            return 7;
        case BUS:
        case MEMORY:
            throw invalid_argument("Invalid component");
    }
    throw invalid_argument("Invalid component");
}
#endif