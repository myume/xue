#include "ArenaAllocator.h"
#include <stdexcept>

ArenaAllocator::ArenaAllocator(size_t capacity)
    : capacity(capacity), memory(malloc(capacity)) {}

ArenaAllocator::~ArenaAllocator() { free(memory); };

ArenaAllocator::ArenaAllocator(ArenaAllocator &&other)
    : capacity(other.capacity), memory(other.memory),
      allocated(other.allocated) {
    other.capacity = 0;
    other.allocated = 0;
    other.memory = nullptr;
};

ArenaAllocator &ArenaAllocator::operator=(ArenaAllocator &&other) {
    if (&other == this) {
        return *this;
    }
    memory = other.memory;
    capacity = other.capacity;
    allocated = other.allocated;
    other.capacity = 0;
    other.allocated = 0;
    other.memory = nullptr;
    return *this;
};

void *ArenaAllocator::alloc(size_t size) {
    if (allocated + size > capacity)
        throw std::runtime_error("out of memory");

    auto ptr = reinterpret_cast<char *>(memory) + allocated;
    allocated += size;
    return ptr;
};

void ArenaAllocator::clear() { allocated = 0; };
