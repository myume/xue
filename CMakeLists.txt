cmake_minimum_required(VERSION 3.25)
project(xue)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(xue STATIC
    src/allocators/ArenaAllocator.cpp
    src/allocators/ArenaAllocator.h
    src/allocators/StackAllocator.cpp
    src/allocators/StackAllocator.h
    src/allocators/DoubleStackAllocator.cpp
    src/allocators/DoubleStackAllocator.h
    src/allocators/PoolAllocator.h
    src/allocators/PoolAllocator.cpp

    src/RingBuffer.h
)

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(tests 
    tests/allocator_test.cpp
    tests/ring_buffer_test.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain xue)

include(CTest)
include(Catch)
catch_discover_tests(tests)
