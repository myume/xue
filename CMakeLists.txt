cmake_minimum_required(VERSION 3.25)
project(xue)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(xue_alloc STATIC
    src/StackAllocator.cpp
    src/StackAllocator.h
)

