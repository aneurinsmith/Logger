﻿
cmake_minimum_required (VERSION 3.12)
project (Logger)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include_directories("include/logger")

set(LOGGER_VERSION 1.0.0)

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
		set(PLATFORM "libx11")
	else()
		message(FATAL_ERROR "Platform is unsuppored")
	endif()
	message(STATUS "Platform detected: ${PLATFORM}")
	add_compile_definitions(${PLATFORM})
endif()



#--------------------#
#  LIBRARY CREATION  #
#--------------------#

# get the relevent source files
file(GLOB_RECURSE logger_src "src/*.h" "src/*.cpp")

# create the liblogger library
add_library (logger STATIC ${logger_src})

# set include directory for header files
target_include_directories(logger 
	PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/logger>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/logger>"
)

# add platform definition to targets file
target_compile_definitions(logger PUBLIC ${PLATFORM})

# set c++ version to 17
target_compile_features(logger PUBLIC cxx_std_17)

# set compile tags to suppress warnings
target_compile_options(logger PUBLIC "-std=c++17")
target_compile_options(logger PUBLIC "-Wno-macro-redefined")
target_compile_options(logger PUBLIC "-Wno-switch")

# link platform libraries to logger
if(PLATFORM STREQUAL "win32")
	target_link_libraries(logger -lcomctl32)
	target_link_libraries(logger -luxtheme)
	target_link_libraries(logger -ldwmapi)
elseif(PLATFORM STREQUAL "libx11")
	target_link_libraries(logger -lX11)
	target_link_libraries(logger -lXft)
	target_link_libraries(logger -lXrandr)
	target_link_libraries(logger -lpthread)
endif()



#----------------------#
#  INSTALLATION RULES  #
#----------------------#

# install libraries and binaries
install(TARGETS logger EXPORT loggerTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# install header files
install(
	DIRECTORY "include/logger"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# install cmake config files
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/loggerConfigVersion.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/loggerConfig.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/logger"
)

# install cmake targets file
install(EXPORT loggerTargets
	FILE "loggerTargets.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/logger"
	NAMESPACE logger::
)



#--------------------------#
#  CONFIG FILE GENERATION  #
#--------------------------#

# export loggerConfigVersion
write_basic_package_version_file(
	"${CMAKE_INSTALL_DATADIR}/loggerConfigVersion.cmake"
	VERSION ${LOGGER_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# export loggerConfig
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config/loggerConfig.cmake.in"
	"${CMAKE_INSTALL_DATADIR}/loggerConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}"
)

# export loggerTargets
export(TARGETS logger
	FILE "${CMAKE_INSTALL_DATADIR}/loggerTargets.cmake"
	NAMESPACE logger::
)



#-----------------------#
#  EXAMPLE EXECUTABLES  #
#-----------------------#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory("examples")
endif()
