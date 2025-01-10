
cmake_minimum_required (VERSION 3.8)
project (Logger)
set(CMAKE_CXX_STANDARD 11)

# Attempt to detect the platform
if(NOT PLATFORM)
	if(WIN32)
		set(PLATFORM "win32")
	elseif(UNIX AND NOT APPLE)
		set(PLATFORM "x11")
	endif()
	message("-- Platform detected (${PLATFORM})")
	add_compile_definitions(${PLATFORM})
endif()

file(GLOB_RECURSE logger_src "src/*.h" "src/*.cpp")
add_library (logger STATIC ${logger_src})

add_subdirectory(tests)
