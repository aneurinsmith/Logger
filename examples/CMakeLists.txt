
add_executable (Timer "timer_test.cpp")
target_link_libraries(Timer logger)

add_executable (FStreamSink "fstream_sink_example.cpp")
target_link_libraries(FStreamSink logger)

add_executable (OStreamSink "ostream_sink_example.cpp")
target_link_libraries(OStreamSink logger)

add_executable (ConsoleSink "console_sink_example.cpp")
target_link_libraries(ConsoleSink logger)
