#include "perf_param.hpp"

// Incrementing the performance statistics
void PerfStats :: incr_back_pressure(){
    backPressure += (ull)1;
}

void PerfStats :: incr_bus_access(){
    busAccess += (ull)1;
}

void PerfStats :: incr_cache_hit (){
    cacheHit += (ull)1;
}

void PerfStats :: incr_cache_miss(){
    cacheMiss += (ull)1;
}

void PerfStats :: incr_mem_access(){
    memAccess += (ull)1;
}

void PerfStats :: incr_mem_write_back(){
    memWriteBack += (ull)1;
}
//---------------------------------------
// Getters and Setters
Stats_t PerfStats :: get_bus_access(){
    return busAccess;
}

Stats_t PerfStats :: get_cache_hit(){
    return cacheHit;
}

Stats_t PerfStats :: get_cache_miss(){
    return cacheMiss;
}

Stats_t PerfStats :: get_mem_access(){
    return memAccess;
}

Stats_t PerfStats :: get_mem_write_back(){
    return memWriteBack;
}

Stats_t PerfStats :: get_back_pressure(){
    return backPressure;
}

Stats_t PerfStats :: get_opcode_count(Opcode opcode){
    return opcodeCount[opcode];
}

void PerfStats :: incr_opcode_count(Opcode opcode){
    opcodeCount[opcode] += (ull)1;
}
void PerfStats :: printPerf(){
    cout << "Performance metrics\n";
    cout << "bus access: " << busAccess << "\n";
    cout << "cache hit: " << cacheHit << "\n";
    cout << "cache miss: " << cacheMiss << "\n";
    cout << "memory access for read: " << memAccess << "\n";
    cout << "memory write-back: " << memWriteBack << "\n";
    cout << "back pressure: " << backPressure << "\n";
    cout << "opcode count:\n";
    for (int i=0; i<Opcode::NUM_OPCODES; i++){
        if (get_opcode_count((Opcode)i) == 0){
            continue;
        }
        cout << convertOpcodeToString((Opcode)i) << ": " << get_opcode_count((Opcode)i) << "\n";
    }
}

void PerfStats :: set_back_pressure(Stats_t x){
    backPressure = x;
}
void PerfStats :: set_bus_access(Stats_t x){
    busAccess = x;
}

void PerfStats :: set_cache_hit(Stats_t x){
    cacheHit = x;
}

void PerfStats :: set_cache_miss(Stats_t x){
    cacheHit = x;
}

void PerfStats :: set_mem_access ( Stats_t x){
    memAccess = x;
}

void PerfStats :: set_mem_write_back ( Stats_t x){
    memWriteBack = x;
}

/// PerfParam class

PerfParam :: PerfParam(){
    parameters[Parameters::CORE_TO_BUS_BUF_SIZE] = 16;
    parameters[Parameters::CORE_TO_BUS_RESP_BUF_SIZE] = 16;
    parameters[Parameters::BUS_TO_CORE_BUF_SIZE] = 16;
    parameters[Parameters::BUS_TO_MEM_BUF_SIZE] = 16;
    parameters[Parameters::MEM_TO_BUS_BUF_SIZE] = 16;
}

Params_t PerfParam :: getParameter(Parameters param){
    return parameters[param];
}

void PerfParam :: setParameter(Parameters param, Params_t value){
    parameters[param] = value;
}

bool PerfParam :: isValidParameter(Parameters param){
    if (parameters.find(param) != parameters.end()){
        return true;
    } else {
        return false;
    }
}

void PerfParam :: printParam(){
    cout << "Performance parameters\n";
    for (auto it = parameters.begin(); it != parameters.end(); ++it){
        cout << convertParameterToString(it->first) << ": " << it->second << "\n";
    }
}

void PerfParam :: setParameterHashMap(std :: unordered_map <Parameters, Params_t> param){
    parameters = param;
}

//PerfBase class

PerfBase :: PerfBase(): backPressure(0){
    for (int i=0; i<Opcode::NUM_OPCODES; i++){
        opcodeCount[i] = 0;
    }
}

void PerfBase :: incr_back_pressure(){
    backPressure += (ull)1;
}
void PerfBase :: incr_opcode_count(Opcode opcode){
    opcodeCount[opcode] += (ull)1;
}

Stats_t PerfBase :: get_opcode_count(Opcode opcode){
    return opcodeCount[opcode];
}

Stats_t PerfBase :: get_back_pressure(){
    return backPressure;
}
//---------------------------------------
// PerfBus class

PerfBus :: PerfBus(): PerfBase(){
}

void PerfBus :: printPerf(){
    cout << "Performance metrics for Bus\n";
    cout << "back pressure: " << get_back_pressure() << "\n";
    cout << "opcode count:\n";
    for (int i=0; i<Opcode::NUM_OPCODES; i++){
        if (get_opcode_count((Opcode)i) == 0){
            continue;
        }
        cout << convertOpcodeToString((Opcode)i) << ": " << get_opcode_count((Opcode)i) << "\n";
    }
}

//---------------------------------------
//PerfMem
PerfMem :: PerfMem(): PerfBase(){
}

void PerfMem :: printPerf(){
    cout << "Performance metrics for Memory\n";
    cout << "back pressure: " << get_back_pressure() << "\n";
    cout << "opcode count:\n";
    for (int i=0; i<Opcode::NUM_OPCODES; i++){
        if (get_opcode_count((Opcode)i) == 0){
            continue;
        }
        cout << convertOpcodeToString((Opcode)i) << ": " << get_opcode_count((Opcode)i) << "\n";
    }
}
//---------------------------------------
