#include "arena.h"
#include <algorithm>
#include <cstdint>

constexpr size_t kBlockSize = 2048 * 1024; // Standard 2MB block

Arena::Arena() = default;

Arena::~Arena() {
    for (char* block : blocks_) {
        delete[] block; // Free all allocated blocks 🧹
    }
}

char* Arena::Allocate(size_t bytes) {
    if (bytes <= alloc_bytes_remaining_) {
        char* result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;
        return result;
    }
    return AllocateFallback(bytes);
}

char* Arena::AllocateAligned(size_t bytes) {
    constexpr size_t align = sizeof(void*);
    size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
    size_t slop = (current_mod == 0) ? 0 : (align - current_mod);
    size_t needed = bytes + slop;

    if (needed <= alloc_bytes_remaining_) {
        char* result = alloc_ptr_ + slop;
        alloc_ptr_ += needed;
        alloc_bytes_remaining_ -= needed;
        return result;
    }
    return AllocateFallback(bytes);
}

char* Arena::AllocateFallback(size_t bytes) {
    if (bytes > kBlockSize / 4) {
        // Large allocations get their own block to prevent wasting remaining space
        return AllocateNewBlock(bytes);
    }
    alloc_ptr_ = AllocateNewBlock(kBlockSize);
    alloc_bytes_remaining_ = kBlockSize;

    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
}

char* Arena::AllocateNewBlock(size_t block_bytes) {
    char* block = new char[block_bytes];
    blocks_.push_back(block);
    memory_usage_.fetch_add(block_bytes, std::memory_order_relaxed);
    return block;
}

size_t Arena::MemoryUsage() const {
    return memory_usage_.load(std::memory_order_relaxed);
}

