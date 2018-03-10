cmake_minimum_required(VERSION 3.7)
project(zad2)

set(CMAKE_C_STANDARD 11)

add_executable(master master.c)
target_link_libraries(master m)

add_executable(slave slave.c)
target_link_libraries(slave m)
