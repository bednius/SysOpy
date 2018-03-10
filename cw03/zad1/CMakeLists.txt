cmake_minimum_required(VERSION 3.6)
project(untitled9)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c interpreter.h interpreter.c)
add_executable(zad1 ${SOURCE_FILES})

