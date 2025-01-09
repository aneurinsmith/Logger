
add_executable (OStreamSink "ostreamsink.cpp")
target_include_directories(OStreamSink PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(OStreamSink logger)

add_executable (TerminalSink "terminalsink.cpp")
target_include_directories(TerminalSink PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(TerminalSink logger)