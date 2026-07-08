#include "StackAllocator.h"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

StackAllocator::StackAllocator(size_t size)
    : size(size), memory(malloc(size)) {}

StackAllocator::~StackAllocator() { free(memory); };

StackAllocator::StackAllocator(StackAllocator &&alloc)
    : memory(alloc.memory), size(alloc.size), allocated(alloc.allocated),
      blocks(alloc.blocks) {
    alloc.memory = nullptr;
    alloc.size = 0;
    alloc.allocated = 0;
    alloc.blocks.clear();
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

    size_t size = blocks.back();

    void *expected = reinterpret_cast<char *>(memory) + allocated - size;
    if (ptr != expected) {
        throw std::runtime_error(
            "freed address match the last allocated pointer");
    }

    allocated -= size;
    blocks.pop_back();
};

void StackAllocator::clear() {
    blocks.clear();
    allocated = 0;
};
