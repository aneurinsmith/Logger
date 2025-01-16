
cmake_minimum_required (VERSION 3.8)
project (Logger)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)
include_directories("include/logger")

# Attempt to detect the platform
if(NOT PLATFORM)
	if(WIN32)
		set(PLATFORM "win32")
	elseif(UNIX AND NOT APPLE)
		set(PLATFORM "x11")
	endif()
	message(STATUS "Platform detected (${PLATFORM})")
	add_compile_definitions(${PLATFORM})
endif()

# Create the library from source code
file(GLOB_RECURSE logger_src "src/*.cpp")
add_library (logger STATIC ${logger_src})

# Create the test executables
add_subdirectory(test)


target_include_directories(logger PUBLIC 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/logger>"
)

# Package
install(TARGETS logger EXPORT loggerTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY "include/logger"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT loggerTargets
	FILE loggerTargets.cmake
	NAMESPACE LOG::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/logger"
)
