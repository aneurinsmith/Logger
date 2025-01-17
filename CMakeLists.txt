
cmake_minimum_required (VERSION 3.8)
project (Logger)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include_directories("include/logger")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(logger_VERSION 0.1.0)
set(logger_INCLUDEDIR "include/logger")
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
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/logger>"
)


#--------------------------#
#  CONFIG FILE GENERATION  #
#--------------------------#
write_basic_package_version_file(
	"${CMAKE_INSTALL_DATADIR}/loggerConfigVersion.cmake"
	VERSION ${logger_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config/loggerConfig.cmake.in"
	"${CMAKE_INSTALL_DATADIR}/loggerConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}"
	PATH_VARS logger_INCLUDEDIR
)


#----------------------#
#  INSTALLATION RULES  #
#----------------------#
install(TARGETS logger EXPORT loggerTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/logger"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/logger"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/logger"
)

install(
	DIRECTORY "include/logger"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/logger"
)

install(FILES 
	"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/loggerConfigVersion.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/loggerConfig.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/logger"
)


#-----------------------#
#  EXAMPLE EXECUTABLES  #
#-----------------------#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory("examples")
endif()
