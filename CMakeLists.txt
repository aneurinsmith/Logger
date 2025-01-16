
cmake_minimum_required (VERSION 3.8)
project (Logger)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include_directories("include/logger")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LOGGER_VERSION 0.0.0)
set(LOGGER_PACKAGE_DIR "${CMAKE_INSTALL_DATADIR}/logger/cmake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


#----------------------#
#  PLATFORM DETECTION  #
#----------------------#
if(NOT PLATFORM)
	if(WIN32)
		set(PLATFORM "win32")
	elseif(UNIX AND NOT APPLE)
		set(PLATFORM "x11")
	endif()
	message(STATUS "Platform detected (${PLATFORM})")
	add_compile_definitions(${PLATFORM})
endif()


#--------------------#
#  LIBRARY CREATION  #
#--------------------#
file(GLOB_RECURSE logger_src "src/*.cpp")
add_library (logger STATIC ${logger_src})
target_include_directories(logger PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/logger>"
	"$<INSTALL_INTERFACE:include/logger>"
)


#----------------------#
#  INSTALLATION RULES  #
#----------------------#
install(TARGETS logger EXPORT loggerTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
	DIRECTORY "include/logger"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


#--------------------------#
#  CONFIG FILE GENERATION  #
#--------------------------#

# Create package version file
write_basic_package_version_file(
	"${LOGGER_PACKAGE_DIR}/loggerConfigVersion.cmake"
	VERSION ${LOGGER_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# Export package config
export(TARGETS logger
	FILE "${LOGGER_PACKAGE_DIR}/loggerConfig.cmake"
	NAMESPACE Logger::
)

# Install package config
install(EXPORT loggerTargets
	DESTINATION "${CMAKE_INSTALL_DATADIR}/logger/cmake"
	NAMESPACE Logger::
)

message(STATUS "Package files written to: ${LOGGER_PACKAGE_DIR}")


#-----------------------#
#  EXAMPLE EXECUTABLES  #
#-----------------------#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory(test)
endif()
