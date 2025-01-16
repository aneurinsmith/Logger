
add_executable (OStreamSink "ostream_sink_test.cpp")
target_link_libraries(OStreamSink logger)

add_executable (TerminalSink "terminal_sink_test.cpp")
target_link_libraries(TerminalSink logger)
