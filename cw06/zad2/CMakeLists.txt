cmake_minimum_required(VERSION 3.7)
project(zad2)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c common.h)
target_link_libraries(server rt)
add_executable(client client.c common.h)
target_link_libraries(client rt)
