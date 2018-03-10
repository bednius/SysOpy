cmake_minimum_required(VERSION 3.6)
project(SO02)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c libf.h libf.c sysf.h sysf.c)
add_executable(zad1 ${SOURCE_FILES})
