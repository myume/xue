#include "StackAllocator.h"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

StackAllocator::StackAllocator(size_t size)
    : size(size), memory(malloc(size)) {}

StackAllocator::~StackAllocator() { free(memory); };

StackAllocator::StackAllocator(StackAllocator &&alloc)
    : memory(alloc.memory), size(alloc.size), allocated(alloc.allocated) {
    alloc.memory = nullptr;
    alloc.size = 0;
};

void *StackAllocator::alloc(size_t size) {
    void *top = reinterpret_cast<char *>(memory) + allocated;
    allocated += size;

    if (allocated > size) {
        throw std::runtime_error("out of memory");
    }

    blocks.push_back(size);
    return top;
}

void StackAllocator::pop_free(void *ptr) {
    size_t size = blocks.back();

    void *expected = reinterpret_cast<char *>(memory) + allocated - size;
    if (ptr != expected) {
        throw std::runtime_error(
            "freed address match the last allocated pointer");
    }

    allocated -= size;
};

void StackAllocator::clear() {
    blocks.clear();
    allocated = 0;
};
