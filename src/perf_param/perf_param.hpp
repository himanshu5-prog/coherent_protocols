#ifndef PERF_PARAM_H
#define PERF_PARAM_H

#include "../data_type.hpp"

class Perf_params {
    long long int busAccess;
    long long int memAccess;
    long long int memWriteBack;

    long long int cacheMiss;
    long long int cacheHit;

    public:

        Perf_params(){
            busAccess = 0;
            memAccess = 0;
            memWriteBack = 0;
            cacheMiss = 0;
            cacheHit = 0;
        }

        void incr_bus_access();
        void incr_mem_access();
        void incr_mem_write_back();
        void incr_cache_miss();
        void incr_cache_hit();

        ll get_bus_access();
        ll get_mem_access();
        ll get_mem_write_back();
        ll get_cache_miss();
        ll get_cache_hit();

        void set_bus_access(ll x);
        void set_mem_access(ll x);
        void set_mem_write_back(ll x);
        void set_cache_miss(ll x);
        void set_cache_hit(ll x);
};
#endif