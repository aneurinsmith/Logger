
cmake_minimum_required (VERSION 3.8)
project (Logger)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE logger_src "src/*.h" "src/*.cpp")
add_library (logger STATIC ${logger_src})

add_subdirectory(tests)
