g++ \
-std=c++17 \
src/bus/bus.cpp \
src/core/core.cpp \
src/cpu/cpu.cpp \
src/memory/memory.cpp \
src/perf_param/perf_param.cpp \
src/processor/processor.cpp \
src/data_type/data_type.cpp \
src/printFunc.cpp \
coherentProtocol.cpp \
-std=c++14 \
-o CoherentProtocol
##./CoherentProtocol input/simple_read.txt -v 0