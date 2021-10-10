cmake_minimum_required(VERSION 3.16)
project(archiver)

set(CMAKE_CXX_STANDARD 20)

add_executable(archiver main.cpp library/compressor.h library/compressor.cpp library/input_process.cpp library/decompressor.cpp library/decompressor.h library/my_heap.cpp library/input_process.h library/my_heap.h)
