#include "PoolAllocator.h"
#include <stdexcept>

FreeLinkedList::FreeLinkedList(size_t blocksize, size_t capacity)
    : FreeList{blocksize, capacity}, head(nullptr) {
    for (auto i = 0; i < capacity; i++) {
        freeBlock(reinterpret_cast<char *>(memory) + blocksize * i);
    }
}

void *FreeLinkedList::nextBlock() {
    if (!head) {
        throw std::runtime_error("out of memory");
    }

    void *block = head->block;
    head = std::move(head->next);
    return block;
}

void FreeLinkedList::freeBlock(void *block) {
    auto offset =
        reinterpret_cast<char *>(block) - reinterpret_cast<char *>(memory);
    if (offset < 0 || offset >= blocksize * capacity ||
        offset % blocksize != 0) {
        throw std::runtime_error("invalid block");
    }

    head = std::make_unique<Node>(block, std::move(head));
}

void *PoolAllocator::nextBlock() { return impl->nextBlock(); };
void PoolAllocator::freeBlock(void *block) { impl->freeBlock(block); };
