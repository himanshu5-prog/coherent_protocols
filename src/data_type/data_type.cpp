#include "data_type.hpp"

Parameters convertStringToParameter( const std :: string &str) {
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

std :: string convertParameterToString(Parameters param) {
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

std :: string convertOpcodeToString (Opcode op){
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
        case ReadInstruction:
            return "ReadInstruction";
        case WriteInstruction:
            return "WriteInstruction";
        case NUM_OPCODES:
            return "NUM_OPCODES";
    }
    throw invalid_argument("Invalid opcode");
}

std :: string convertCacheStateToString (CacheState state){
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

std :: string convertComponentToString (Component comp){
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

Component convertCoreIDToComponent (int coreID){
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

int convertComponentToCoreID (Component comp){
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