cmake_minimum_required(VERSION 3.13)
project(Integral)

set(CMAKE_CXX_STANDARD 14)

find_package( Threads )
add_executable(Integral src/main.cpp src/config_reader.h)
target_link_libraries( Integral  ${CMAKE_THREAD_LIBS_INIT} )