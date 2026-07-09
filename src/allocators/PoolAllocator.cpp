#include "PoolAllocator.h"
#include <stdexcept>

PoolAllocator::PoolAllocator(size_t blockSize, size_t capacity)
    : blockSize(blockSize), capacity(capacity), head(nullptr) {
    if (blockSize < sizeof(void *)) {
        throw std::runtime_error("minimum block size is size of pointer");
    }

    memory = malloc(blockSize * capacity);
    for (auto i = 0; i < capacity; i++) {
        auto addr = reinterpret_cast<char *>(memory) + i * blockSize;
        freeBlock(addr);
    }
};

PoolAllocator::~PoolAllocator() { free(memory); };

void PoolAllocator::freeBlock(void *block) {
    auto offset =
        reinterpret_cast<char *>(block) - reinterpret_cast<char *>(memory);
    if (offset < 0 || offset >= blockSize * capacity ||
        offset % blockSize != 0) {
        throw std::runtime_error("invalid block");
    }

    auto addr = reinterpret_cast<void **>(block);
    *addr = head;
    head = addr;
};

[[nodiscard]] void *PoolAllocator::nextBlock() {
    if (!head)
        throw std::runtime_error("out of memory");
    auto freeBlock = head;
    head = reinterpret_cast<void **>(*head);
    return freeBlock;
};

PoolAllocator::PoolAllocator(PoolAllocator &&other) noexcept
    : memory(other.memory), blockSize(other.blockSize),
      capacity(other.capacity), head(other.head) {
    other.memory = nullptr;
    other.head = nullptr;
    other.blockSize = 0;
    other.capacity = 0;
};

PoolAllocator &PoolAllocator::operator=(PoolAllocator &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    free(memory);
    memory = other.memory;
    head = other.head;
    capacity = other.capacity;
    blockSize = other.blockSize;
    other.memory = nullptr;
    other.head = nullptr;
    other.blockSize = 0;
    other.capacity = 0;
    return *this;
};
