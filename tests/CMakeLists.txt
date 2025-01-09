
add_executable (OStreamSink "ostreamsink.cpp")
target_include_directories(OStreamSink PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(OStreamSink logger)