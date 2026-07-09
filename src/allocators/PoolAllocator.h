#pragma once

#include <cstdlib>

class PoolAllocator {
  private:
    void *memory;
    void **head;
    size_t blockSize;
    size_t capacity;

  public:
    /// Construct a new PoolAllocator where each block is [blockSize] and can
    /// allocate [capacity] in total.
    PoolAllocator(size_t blockSize, size_t capacity);
    ~PoolAllocator();

    PoolAllocator(PoolAllocator &&) noexcept;
    PoolAllocator &operator=(PoolAllocator &&) noexcept;

    PoolAllocator(const PoolAllocator &) = delete;
    PoolAllocator &operator=(const PoolAllocator &) = delete;

    [[nodiscard]] void *nextBlock();

    void freeBlock(void *block);
};
