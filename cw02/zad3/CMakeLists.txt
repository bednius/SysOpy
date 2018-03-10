cmake_minimum_required(VERSION 3.6)
project(untitled8)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c locks.h locks.c)
add_executable(zad3 ${SOURCE_FILES})
