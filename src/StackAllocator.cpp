#include "StackAllocator.h"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

StackAllocator::StackAllocator(size_t size)
    : size(size), memory(malloc(size)) {}

StackAllocator::~StackAllocator() { free(memory); };

StackAllocator::StackAllocator(StackAllocator &&other) noexcept
    : memory(other.memory), size(other.size), allocated(other.allocated),
      blocks(std::move(other.blocks)) {
    other.memory = nullptr;
    other.size = 0;
    other.allocated = 0;
};

StackAllocator &StackAllocator::operator=(StackAllocator &&other) {
    if (&other == this) {
        return *this;
    }

    free(memory);
    memory = other.memory;
    size = other.size;
    allocated = other.allocated;
    blocks = std::move(other.blocks);
    other.memory = nullptr;
    other.size = 0;
    other.allocated = 0;
    return *this;
};

void *StackAllocator::alloc(size_t bytes) {
    void *top = reinterpret_cast<char *>(memory) + allocated;

    if (allocated + bytes > size) {
        throw std::runtime_error("out of memory");
    }

    allocated += bytes;
    blocks.push_back(bytes);

    return top;
}

void StackAllocator::pop_free(void *ptr) {
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
