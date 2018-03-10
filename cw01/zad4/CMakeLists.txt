cmake_minimum_required(VERSION 3.6)
project(zad4)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES ../zad1/address_book.c ../zad1/address_book.h main.c)
add_executable(zad4 ${SOURCE_FILES})
