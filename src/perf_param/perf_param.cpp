#include "perf_param.hpp"

void Perf_params :: incr_bus_access(){
    busAccess += (ll)1;
}

void Perf_params :: incr_cache_hit (){
    cacheHit += (ll)1;
}

void Perf_params :: incr_cache_miss(){
    cacheMiss += (ll)1;
}

void Perf_params :: incr_mem_access(){
    memAccess += (ll)1;
}

void Perf_params :: incr_mem_write_back(){
    memWriteBack += (ll)1;
}

ll Perf_params :: get_bus_access(){
    return busAccess;
}

ll Perf_params :: get_cache_hit(){
    return cacheHit;
}

ll Perf_params :: get_cache_miss(){
    return cacheMiss;
}

ll Perf_params :: get_mem_access(){
    return memAccess;
}

ll Perf_params :: get_mem_write_back(){
    return memWriteBack;
}

void Perf_params :: set_bus_access(ll x){
    busAccess = x;
}

void Perf_params :: set_cache_hit(ll x){
    cacheHit = x;
}

void Perf_params :: set_cache_miss(ll x){
    cacheHit = x;
}

void Perf_params :: set_mem_access ( ll x){
    memAccess = x;
}

void Perf_params :: set_mem_write_back ( ll x){
    memWriteBack = x;
}