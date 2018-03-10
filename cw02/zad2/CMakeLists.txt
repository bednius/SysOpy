cmake_minimum_required(VERSION 3.6)
project(cw02zad2)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILESa zad2a.c statf.h statf.c)
set(SOURCE_FILESb zad2b.c nftwf.h nftwf.c)

add_executable(zad2a ${SOURCE_FILESa})
add_executable(zad2b ${SOURCE_FILESb})
