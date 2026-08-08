#pragma once
#include <vector>
#include <cstddef>
#include <atomic>

class Arena {
public:
    Arena();
    ~Arena();

    // Disallow copying to prevent duplicate memory ownership
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    // Allocates 'bytes' of memory
    char* Allocate(size_t bytes);

    // Allocates 8-byte aligned memory for pointers/atomics
    char* AllocateAligned(size_t bytes);

    // Returns total memory allocated across all blocks
    size_t MemoryUsage() const;

private:
    char* AllocateFallback(size_t bytes);
    char* AllocateNewBlock(size_t block_bytes);

    char* alloc_ptr_{nullptr};           // Bump pointer to current free position
    size_t alloc_bytes_remaining_{0};     // Remaining free space in current block
    std::vector<char*> blocks_;           // Tracks all allocated memory blocks
    std::atomic<size_t> memory_usage_{0}; // Total bytes allocated from OS
};