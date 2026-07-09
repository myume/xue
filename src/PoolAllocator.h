#pragma once

#include <cstdlib>
#include <memory>

class FreeList {
  protected:
    void *memory;
    size_t capacity;
    size_t blocksize;

  public:
    FreeList(size_t blocksize, size_t capacity)
        : memory(malloc(blocksize * capacity)), blocksize(blocksize),
          capacity(capacity) {};
    virtual ~FreeList() { free(memory); };

    virtual void *nextBlock() = 0;
    virtual void freeBlock(void *block) = 0;
};

class FreeLinkedList : public FreeList {
  private:
    class Node {
      public:
        Node(void *block, std::unique_ptr<Node> next)
            : block(block), next(std::move(next)) {};

        void *block;
        std::unique_ptr<Node> next;
    };

    std::unique_ptr<Node> head;

  public:
    FreeLinkedList(size_t blocksize, size_t capacity);

    void *nextBlock() override;

    void freeBlock(void *block) override;
};

class PoolAllocator {
  private:
    std::unique_ptr<FreeList> impl;

  public:
    /// Construct a new PoolAllocator where each block is [blockSize] and can
    /// allocate [capacity] in total.
    PoolAllocator(size_t blockSize, size_t capacity) {
        impl = std::make_unique<FreeLinkedList>(blockSize, capacity);
    };

    [[nodiscard]] void *nextBlock();

    void freeBlock(void *block);
};
