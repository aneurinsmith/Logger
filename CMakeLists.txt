
cmake_minimum_required (VERSION 3.8)
project (Logger)

add_library (logger STATIC "src/logger.cpp" "src/logger.h")

add_subdirectory(tests)
