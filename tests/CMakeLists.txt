
add_executable (StaticTest "main.cpp")
target_include_directories(StaticTest PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(StaticTest logger)