#include "../src/StackAllocator.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("allocates correct capacity", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    REQUIRE_NOTHROW(alloc.alloc(16));
    REQUIRE_THROWS(alloc.alloc(8));
}

TEST_CASE("allocates blocks correctly", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(8);
    auto b = alloc.alloc(8);

    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    REQUIRE(a != b);
}

TEST_CASE("frees blocks correctly", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(16);
    alloc.pop_free(a);
    auto b = alloc.alloc(16);
    REQUIRE(a == b);
}

TEST_CASE("frees blocks in correct order", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(8);
    auto b = alloc.alloc(8);
    REQUIRE_THROWS(alloc.pop_free(a));
    REQUIRE_NOTHROW(alloc.pop_free(b));
    REQUIRE_NOTHROW(alloc.pop_free(a));
}

TEST_CASE("clears blocks correctly", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(8);
    auto b = alloc.alloc(8);

    alloc.clear();
    auto c = alloc.alloc(16);

    REQUIRE(c == a);
    REQUIRE_THROWS(alloc.pop_free(b));
}

TEST_CASE("move constructor transfers ownership and blocks",
          "[StackAllocator]") {
    StackAllocator a(64);
    void *p = a.alloc(32);
    StackAllocator b(std::move(a));
    REQUIRE_NOTHROW(b.pop_free(p));
}

TEST_CASE("pop on emty stack allocator throws", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    REQUIRE_THROWS(alloc.pop_free(nullptr));
}
