cmake_minimum_required(VERSION 3.7)
project(zad1)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c common.h)
add_executable(client client.c common.h)
