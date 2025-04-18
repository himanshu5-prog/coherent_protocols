#ifndef PERF_PARAM_H
#define PERF_PARAM_H

#include "../data_type/data_type.hpp"

//Stats for Core class
class PerfStats {
    Stats_t busAccess;
    Stats_t memAccess;
    Stats_t memWriteBack;

    Stats_t cacheMiss;
    Stats_t cacheHit;
    
    //Number of times back pressure is generated
    Stats_t backPressure;

    // Opcode count for RX and TX
    // RX: Bus to Core, TX: Core to Bus
    Stats_t opcodeCount_rx[Opcode::NUM_OPCODES];
    Stats_t opcodeCount_tx[Opcode::NUM_OPCODES];

    public:

        PerfStats(){
            busAccess = 0;
            memAccess = 0;
            memWriteBack = 0;
            cacheMiss = 0;
            cacheHit = 0;
            backPressure = 0;

            for (int i=0; i<Opcode::NUM_OPCODES; i++){
                opcodeCount_rx[i] = 0;
                opcodeCount_tx[i] = 0;
            }
        }

        void incr_bus_access();
        void incr_mem_access();
        void incr_mem_write_back();
        void incr_cache_miss();
        void incr_cache_hit();
        void incr_back_pressure();
        void incr_opcode_count_rx(Opcode op);
        void incr_opcode_count_tx(Opcode op);

        Stats_t get_bus_access();
        Stats_t get_mem_access();
        Stats_t get_mem_write_back();
        Stats_t get_cache_miss();
        Stats_t get_cache_hit();
        Stats_t get_back_pressure();
        Stats_t get_opcode_count_tx(Opcode op);
        Stats_t get_opcode_count_rx(Opcode op);

        void set_bus_access(Stats_t x);
        void set_mem_access(Stats_t x);
        void set_mem_write_back(Stats_t x);
        void set_cache_miss(Stats_t x);
        void set_cache_hit(Stats_t x);
        void set_back_pressure(Stats_t x);
        void printPerf();
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


class PerfBase{
    Stats_t opcodeCountRx[Opcode::NUM_OPCODES];
    Stats_t opcodeCountTx[Opcode::NUM_OPCODES];
    Stats_t backPressure;
    
    public:
        PerfBase();
        void incr_opcode_count_rx(Opcode op);
        void incr_opcode_count_tx(Opcode op);
        void incr_back_pressure();
        Stats_t get_opcode_count_rx(Opcode op);
        Stats_t get_opcode_count_tx(Opcode op);
        Stats_t get_back_pressure();
        virtual void printPerf() = 0;
};

class PerfBus : public PerfBase {
    public:
        PerfBus();
        void printPerf();

};

class PerfMem : public PerfBase {
    public:
        PerfMem();
        void printPerf();
};


#endif