cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("Welcome to CPP Concurrency in Action!")
project(cpp-concurrency LANGUAGES CXX)

include(dependencies/get_cpm.cmake)
include(dependencies/dependencies.cmake)

add_subdirectory(hello_concurrency)
add_subdirectory(basic_threads)
add_subdirectory(pipeline)