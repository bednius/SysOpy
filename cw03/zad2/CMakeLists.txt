cmake_minimum_required(VERSION 3.6)
project(zad2)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c interpreter.h interpreter.c)
add_executable(zad2 ${SOURCE_FILES})

add_executable(malloc malloc.c)

add_executable(endless endless.c)
