cmake_minimum_required (VERSION 3.6)
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic")
project (zad1)

set (SOURCE_FILES address_book.c address_book.h)


add_library (address_book STATIC ${SOURCE_FILES})
add_library (address_book_shared SHARED ${SOURCE_FILES})

