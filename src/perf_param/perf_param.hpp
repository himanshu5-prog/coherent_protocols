#ifndef PERF_PARAM_H
#define PERF_PARAM_H

#include "../data_type.hpp"

class PerfStats {
    Stats_t busAccess;
    Stats_t memAccess;
    Stats_t memWriteBack;

    Stats_t cacheMiss;
    Stats_t cacheHit;

    public:

        PerfStats(){
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

        Stats_t get_bus_access();
        Stats_t get_mem_access();
        Stats_t get_mem_write_back();
        Stats_t get_cache_miss();
        Stats_t get_cache_hit();

        void set_bus_access(Stats_t x);
        void set_mem_access(Stats_t x);
        void set_mem_write_back(Stats_t x);
        void set_cache_miss(Stats_t x);
        void set_cache_hit(Stats_t x);
};

class PerfParam {
    std :: unordered_map <Parameters, Params_t> parameters;

    public:
        PerfParam();
        Params_t getParameter(Parameters param);
        void setParameter(Parameters param, Params_t value);
        void printParam();
        bool isValidParameter(Parameters param);
        void setParameterHashMap(std :: unordered_map <Parameters, Params_t> param);
};
#endif