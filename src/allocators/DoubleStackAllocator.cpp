#include "DoubleStackAllocator.h"
#include <cstddef>
#include <stdexcept>

DoubleStackAllocator::DoubleStackAllocator(size_t capacity)
    : memory(malloc(capacity)), high(capacity), capacity(capacity) {}

DoubleStackAllocator::~DoubleStackAllocator() { free(memory); };

DoubleStackAllocator::DoubleStackAllocator(
    DoubleStackAllocator &&other) noexcept
    : high(other.capacity), low(other.low), capacity(other.capacity),
      memory(other.memory) {
    other.memory = nullptr;
    other.high = 0;
    other.low = 0;
    other.capacity = 0;
}

DoubleStackAllocator &
DoubleStackAllocator::operator=(DoubleStackAllocator &&other) {
    if (&other == this) {
        return *this;
    }

    free(memory);
    memory = other.memory;
    high = other.high;
    low = other.low;
    capacity = other.capacity;

    other.memory = nullptr;
    other.high = 0;
    other.low = 0;
    other.capacity = 0;
    return *this;
};

void *DoubleStackAllocator::allocLow(size_t size) {
    if (low + size > high) {
        throw std::runtime_error("stack overflow");
    }

    auto addr = reinterpret_cast<char *>(memory) + low;
    low += size;
    return addr;
};

void *DoubleStackAllocator::allocHigh(size_t size) {
    if (high - size < low) {
        throw std::runtime_error("stack overflow");
    }

    high -= size;
    return reinterpret_cast<char *>(memory) + high;
};

DoubleStackAllocator::Marker DoubleStackAllocator::lowMarker() {
    return Marker(false, low);
};

DoubleStackAllocator::Marker DoubleStackAllocator::highMarker() {
    return Marker(true, high);
};

void DoubleStackAllocator::clear() noexcept {
    clearLower();
    clearUpper();
};

void DoubleStackAllocator::clearUpper() noexcept { high = capacity; };

void DoubleStackAllocator::clearLower() noexcept { low = 0; };

void DoubleStackAllocator::freeToMarker(Marker mark) {
    if (mark.high) {
        freeToHighMarker(mark);
    } else {
        freeToLowMarker(mark);
    }
};

void DoubleStackAllocator::freeToLowMarker(Marker mark) {
    if (mark.value < low)
        throw std::runtime_error("invalid marker");
    low = mark.value;
};

void DoubleStackAllocator::freeToHighMarker(Marker mark) {
    if (mark.value > high)
        throw std::runtime_error("invalid marker");
    high = mark.value;
};
