#include "StackAllocator.h"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

StackAllocator::StackAllocator(size_t size)
    : capacity(size), memory(malloc(size)) {}

StackAllocator::~StackAllocator() {
    if (memory != nullptr)
        free(memory);
};

StackAllocator::StackAllocator(StackAllocator &&other) noexcept
    : memory(other.memory), capacity(other.capacity),
      allocated(other.allocated), blocks(std::move(other.blocks)) {
    other.memory = nullptr;
    other.capacity = 0;
    other.allocated = 0;
};

StackAllocator &StackAllocator::operator=(StackAllocator &&other) {
    if (&other == this) {
        return *this;
    }

    free(memory);
    memory = other.memory;
    capacity = other.capacity;
    allocated = other.allocated;
    blocks = std::move(other.blocks);
    other.memory = nullptr;
    other.capacity = 0;
    other.allocated = 0;
    return *this;
};

void *StackAllocator::alloc(size_t bytes) {
    void *top = reinterpret_cast<char *>(memory) + allocated;

    if (allocated + bytes > capacity) {
        throw std::runtime_error("out of memory");
    }

    allocated += bytes;
    blocks.push_back(bytes);

    return top;
}

void StackAllocator::pop(void *ptr) {
    if (blocks.empty()) {
        throw std::runtime_error("pop on empty stack");
    }

    size_t bytes = blocks.back();

    void *expected = reinterpret_cast<char *>(memory) + allocated - bytes;
    if (ptr != expected) {
        throw std::runtime_error(
            "freed address match the last allocated pointer");
    }

    allocated -= bytes;
    blocks.pop_back();
};

void StackAllocator::clear() {
    blocks.clear();
    allocated = 0;
};

StackAllocator::Marker StackAllocator::marker() noexcept {
    return Marker(allocated);
};

void StackAllocator::freeToMarker(Marker mark) {
    size_t blocksPopped = 0;
    int delta = allocated - mark.offset;
    for (auto i = blocks.rbegin();
         i + blocksPopped < blocks.rend() && delta > 0; blocksPopped++) {
        delta -= *(i + blocksPopped);
    }

    if (delta != 0)
        throw std::runtime_error("invalid marker");

    allocated = mark.offset;

    for (; blocksPopped > 0; blocksPopped--)
        blocks.pop_back();
};
