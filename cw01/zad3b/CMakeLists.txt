cmake_minimum_required(VERSION 3.6)
project(zad3a)
set(CMAKE_C_STANDARD 99)

option(OPT "OPT" -O2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall ${OPTIMIZATION} -Wpedantic")

set(SOURCE_FILES main.c)
set(SOURCE_FILES_DYNAMIC maindyn.c)

add_executable(zad3astatic ${SOURCE_FILES})
add_executable(zad3ashared ${SOURCE_FILES})
add_executable(zad3adynamic ${SOURCE_FILES_DYNAMIC})

find_library(LIB_LOCATION address_book HINTS ../zad1)
find_library(LIB_LOCATION_SHARED address_book_shared HINTS ../zad1)

target_link_libraries(zad3astatic ${LIB_LOCATION})
target_link_libraries(zad3ashared ${LIB_LOCATION_SHARED})
target_link_libraries(zad3adynamic ${CMAKE_DL_LIBS})

