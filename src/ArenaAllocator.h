#pragma once

#include <cstddef>
#include <cstdlib>

class ArenaAllocator {
  private:
    void *memory;
    size_t allocated = 0;
    size_t capacity;

  public:
    ArenaAllocator(size_t capacity);
    ~ArenaAllocator();

    ArenaAllocator(ArenaAllocator &&);
    ArenaAllocator &operator=(ArenaAllocator &&);

    ArenaAllocator(const ArenaAllocator &) = delete;
    ArenaAllocator &operator=(const ArenaAllocator &) = delete;

    /// Allocate memory with the arena allocator
    void *alloc(size_t size);

    /// Free all memory allocated by the arena allocator
    void clear();
};
