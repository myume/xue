#include "../src/RingBuffer.h"
#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>

TEST_CASE("buffer capacity is correct", "[RingBuffer]") {
    auto buf = RingBuffer<int, 10>();
    for (auto i = 0; i < 10; i++) {
        REQUIRE_NOTHROW(buf.write(i));
    }
    REQUIRE_THROWS(buf.write(10));
}

TEST_CASE("throws when reading from empty buffer", "[RingBuffer]") {
    auto buf = RingBuffer<int, 10>();
    REQUIRE_THROWS(buf.read());
}

TEST_CASE("correctly reads written values", "[RingBuffer]") {
    auto buf = RingBuffer<int, 10>();
    for (auto i = 0; i < 10; i++) {
        auto val = rand() % 101;
        buf.write(val);
        REQUIRE(buf.read() == val);
    }
    REQUIRE_THROWS(buf.read());
}

TEST_CASE("correctly wraps around", "[RingBuffer]") {
    auto buf = RingBuffer<int, 10>();
    for (auto i = 0; i < 100; i++) {
        auto val = rand() % 101;
        buf.write(val);
        REQUIRE(buf.read() == val);
    }
}
