
add_executable (OStreamSink "ostream_sink_test.cpp")
target_include_directories(OStreamSink PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(OStreamSink logger)

add_executable (TerminalSink "terminal_sink_test.cpp")
target_include_directories(TerminalSink PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(TerminalSink logger)
