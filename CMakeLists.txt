cmake_minimum_required(VERSION 3.20)
project(udp_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(udp_server main.cpp )
target_link_libraries(udp_server ws2_32.lib)
