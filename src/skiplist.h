#pragma once
#include <atomic>
#include <cstdint>
#include <new>
#include <random>
#include "arena.h"

template <typename Key, typename Value>
struct Node {
    Key const key;
    Value value;
    uint8_t const height;
    std::atomic<Node*> next[1];

    Node(const Key& k, const Value& v, uint8_t h)
        : key(k), value(v), height(h) {
        for (int i = 0; i < height; ++i) {
            next[i].store(nullptr, std::memory_order_relaxed);
        }
    }
};

template <typename Key, typename Value>
class SkipList {
public:
    static constexpr int kMaxHeight = 12;

    explicit SkipList(Arena* arena)
        : arena_(arena), head_(NewNode(Key(), Value(), kMaxHeight)) {}

    void Insert(const Key& key, const Value& value) {
        Node<Key, Value>* prev[kMaxHeight];
        Node<Key, Value>* candidate = FindGreaterOrEqual(key, prev);

        // If key exists, update value (or handle duplicate key)
        if (candidate != nullptr && candidate->key == key) {
            candidate->value = value;
            return;
        }


        Node<Key, Value>* x = NewNode(key, value, height);

        // Link level 0 first (Logical Insertion)
        while (true) {
            Node<Key, Value>* next = prev[0]->next[0].load(std::memory_order_relaxed);
            x->next[0].store(next, std::memory_order_relaxed);
            if (prev[0]->next[0].compare_exchange_weak(
                    next, x, std::memory_order_release, std::memory_order_relaxed)) {
                break;
            }
            // Re-find predecessors on CAS failure
            FindGreaterOrEqual(key, prev);
        }

        // Link higher levels
        for (int i = 1; i < height; ++i) {
            while (true) {
                Node<Key, Value>* next = prev[i]->next[i].load(std::memory_order_relaxed);
                x->next[i].store(next, std::memory_order_relaxed);
                if (prev[i]->next[i].compare_exchange_weak(
                        next, x, std::memory_order_release, std::memory_order_relaxed)) {
                    break;
                }
                FindGreaterOrEqual(key, prev);
            }
        }

        int height = RandomHeight();
        int current_max = max_height_.load(std::memory_order_release);
        while (height > current_max) {
            if (max_height_.compare_exchange_weak(current_max, height,
                                                 std::memory_order_relaxed)) {
                break;
            }
        }
    }

    bool Contains(const Key& key) const {
        Node<Key, Value>* x = FindGreaterOrEqual(key, nullptr);
        return x != nullptr && x->key == key;
    }

private:
    Node<Key, Value>* NewNode(const Key& key, const Value& value, int height) {
        size_t bytes = sizeof(Node<Key, Value>) + (height - 1) * sizeof(std::atomic<Node<Key, Value>*>);
        char* mem = arena_->AllocateAligned(bytes);
        return new (mem) Node<Key, Value>(key, value, height);
    }

    int RandomHeight() {
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::bernoulli_distribution coin_flip(0.25); // p = 1/4

    int height = 1;
    while (height < kMaxHeight && coin_flip(generator)) {
        height++;
    }
    return height;
}

    Node<Key, Value>* FindGreaterOrEqual(const Key& key, Node<Key, Value>** prev) const {
        Node<Key, Value>* current = head_;
        int level = max_height_.load(std::memory_order_acquire) - 1;

        while (true) {
            Node<Key, Value>* next = current->next[level].load(std::memory_order_acquire);
            if (next != nullptr && next->key < key) {
                current = next;
            } else {
                if (prev != nullptr) {
                    prev[level] = current;
                }
                if (level == 0) {
                    return next;
                }
                level--;
            }
        }
    }

    Arena* arena_;
    Node<Key, Value>* head_;
    std::atomic<int> max_height_{1};
};