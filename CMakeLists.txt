
cmake_minimum_required (VERSION 3.8)
project (Logger)
set(CMAKE_CXX_STANDARD 11)

add_library (logger STATIC "src/logger.cpp" "src/logger.h" "src/sinks/base_sink.h"  )

add_subdirectory(tests)
