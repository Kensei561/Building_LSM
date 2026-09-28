#include <iostream>
#include "arena.h"
#include "skiplist.h"
#include <cstdint>
#include <atomic>
#include <string>

class memtable
{

    enum ValueType : uint8_t
    {
        kTypeDeletion = 0x0,
        kTypeValue = 0x1

    };
    enum Status : uint8_t
    {
        OK = 0x1,
        NotFound = 0x0
    };

    public: 
    memtable();
    ~memtable();
    class LookupKey {};
    void Add(uint64_t sequence, ValueType type, const std::string& key, const std::string& value);
    bool Get(const LookupKey& key, std::string* value, Status* status);
    size_t ApproximateMemoryUsage();

    private:
    explicit memtable(const memtable&) = delete;
    memtable& operator=(const memtable&) = delete;
    Arena arena;
    SkipList<std::string, std::string> skiplist;
    std::atomic<size_t> memory_usage;
};