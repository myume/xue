#pragma once

#include <cstddef>
#include <vector>

/// An allocator that allocates blocks of memory in FIFO order
class StackAllocator {
  public:
    explicit StackAllocator(size_t size);
    ~StackAllocator();

    StackAllocator(StackAllocator &&);
    StackAllocator &operator=(StackAllocator &&);

    // Disallow copy, doesn't make sense here
    StackAllocator(const StackAllocator &) = delete;
    StackAllocator &operator=(const StackAllocator &) = delete;

    /// Allocate memory with the stack allocator of the target size
    void *alloc(size_t size);

    /// Free the pointer allocated with alloc
    void pop_free(void *ptr);

    /// Clear all allocated memory, using previously allocated pointers after
    /// this is undefined behavior
    void clear();

  private:
    void *memory;

    size_t size;
    size_t allocated = 0;
    std::vector<size_t> blocks;
};
