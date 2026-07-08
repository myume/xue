#include "../src/ArenaAllocator.h"
#include "../src/StackAllocator.h"
#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>

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
    alloc.pop(a);
    auto b = alloc.alloc(16);
    REQUIRE(a == b);
}

TEST_CASE("frees blocks in correct order", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(8);
    auto b = alloc.alloc(8);
    REQUIRE_THROWS(alloc.pop(a));
    REQUIRE_NOTHROW(alloc.pop(b));
    REQUIRE_NOTHROW(alloc.pop(a));
}

TEST_CASE("clears blocks correctly", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    auto a = alloc.alloc(8);
    auto b = alloc.alloc(8);

    alloc.clear();
    auto c = alloc.alloc(16);

    REQUIRE(c == a);
    REQUIRE_THROWS(alloc.pop(b));
}

TEST_CASE("move constructor transfers ownership and blocks",
          "[StackAllocator]") {
    StackAllocator a(64);
    void *p = a.alloc(32);
    StackAllocator b(std::move(a));
    REQUIRE_NOTHROW(b.pop(p));
}

TEST_CASE("move assignment transfers ownership and blocks",
          "[StackAllocator]") {
    StackAllocator a(64);
    StackAllocator b(32);
    void *p = a.alloc(32);
    b = std::move(a);
    REQUIRE_NOTHROW(b.pop(p));
}

TEST_CASE("pop on emty stack allocator throws", "[StackAllocator]") {
    auto alloc = StackAllocator(16);
    REQUIRE_THROWS(alloc.pop(nullptr));
}

TEST_CASE("rewinds to marker correctly", "[StackAllocator]") {
    auto alloc = StackAllocator(64);
    auto a = alloc.alloc(32);
    auto mark = alloc.marker();

    auto b = alloc.alloc(8);
    alloc.alloc(16);

    alloc.freeToMarker(mark);
    auto c = alloc.alloc(16);

    REQUIRE(b == c);

    // only c and a should be on the stack
    REQUIRE_NOTHROW(alloc.pop(c));
    REQUIRE_NOTHROW(alloc.pop(a));
    REQUIRE_THROWS(alloc.pop(nullptr));
}

TEST_CASE("throws error when marker is invalid", "[StackAllocator]") {
    SECTION("below marker") {
        auto alloc = StackAllocator(64);
        auto a = alloc.alloc(32);
        auto mark = alloc.marker();
        alloc.pop(a);
        alloc.alloc(8);

        // below previous marker
        REQUIRE_THROWS(alloc.freeToMarker(mark));
    }

    SECTION("invalidated previous memory") {
        auto alloc = StackAllocator(64);
        auto a = alloc.alloc(32);
        auto mark = alloc.marker();
        alloc.pop(a);
        alloc.alloc(8);
        alloc.alloc(32);

        // above previous marker but invalidated previous memory
        // freeing would truncate the second 32 byte block
        REQUIRE_THROWS(alloc.freeToMarker(mark));
    }
}

TEST_CASE("allocates correct capacity", "[ArenaAllocator]") {
    auto alloc = ArenaAllocator(32);
    REQUIRE_NOTHROW(alloc.alloc(16));
    REQUIRE_NOTHROW(alloc.alloc(16));
    REQUIRE_THROWS(alloc.alloc(16));
}

TEST_CASE("frees all memory", "[ArenaAllocator]") {
    auto alloc = ArenaAllocator(32);
    auto start = alloc.alloc(16);
    alloc.alloc(16);
    alloc.free();

    assert(alloc.alloc(8) == start);
}

TEST_CASE("correctly handles allocations", "[ArenaAllocator]") {
    auto alloc = ArenaAllocator(32);
    auto a = reinterpret_cast<int32_t *>(alloc.alloc(4));
    auto b = reinterpret_cast<int32_t *>(alloc.alloc(4));

    *a = 10;
    *b = 12;

    REQUIRE(*a == 10);
    REQUIRE(*b == 12);
}
