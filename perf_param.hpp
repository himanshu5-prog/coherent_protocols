#ifndef PERF_PARAM_H
#define PERF_PARAM_H

class perf_params {
    long long int busAccess;
    long long int memAccess;
    long long int memWriteBack;

    long long int cacheMiss;
    long long int cacheHit;
    
    perf_params(){
        busAccess = 0;
        memAccess = 0;
        memWriteBack = 0;
        cacheMiss = 0;
        cacheHit = 0;
    }
};
#endif