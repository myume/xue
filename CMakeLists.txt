cmake_minimum_required(VERSION 3.25)
project(xue)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(xue_alloc STATIC
    src/ArenaAllocator.cpp
    src/ArenaAllocator.h
    src/StackAllocator.cpp
    src/StackAllocator.h
    src/DoubleStackAllocator.cpp
    src/DoubleStackAllocator.h
)

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(tests tests/allocator_test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain xue_alloc)

include(CTest)
include(Catch)
catch_discover_tests(tests)
