#include "perf_param.hpp"

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