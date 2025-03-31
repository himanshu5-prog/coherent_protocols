#include "perf_param.hpp"

void Perf_params :: incr_bus_access(){
    busAccess += (ull)1;
}

void Perf_params :: incr_cache_hit (){
    cacheHit += (ull)1;
}

void Perf_params :: incr_cache_miss(){
    cacheMiss += (ull)1;
}

void Perf_params :: incr_mem_access(){
    memAccess += (ull)1;
}

void Perf_params :: incr_mem_write_back(){
    memWriteBack += (ull)1;
}

Stats_t Perf_params :: get_bus_access(){
    return busAccess;
}

Stats_t Perf_params :: get_cache_hit(){
    return cacheHit;
}

Stats_t Perf_params :: get_cache_miss(){
    return cacheMiss;
}

Stats_t Perf_params :: get_mem_access(){
    return memAccess;
}

Stats_t Perf_params :: get_mem_write_back(){
    return memWriteBack;
}

void Perf_params :: set_bus_access(Stats_t x){
    busAccess = x;
}

void Perf_params :: set_cache_hit(Stats_t x){
    cacheHit = x;
}

void Perf_params :: set_cache_miss(Stats_t x){
    cacheHit = x;
}

void Perf_params :: set_mem_access ( Stats_t x){
    memAccess = x;
}

void Perf_params :: set_mem_write_back ( Stats_t x){
    memWriteBack = x;
}