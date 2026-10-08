#pragma once
#include "arena.h"
#include "skiplist.h"
#include <cstdint>
#include <atomic>
#include <string>

class memtable
{
    public: 
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
    memtable();
    ~memtable();
    class LookupKey {
        char space_[200];
        char* start_ = space_;
        const char* kstart_;
        const char* end_;

        public:
        const char* mem() const
        {

            return (*this).start_;
        }
        size_t size() const
        {
            return (end_ - start_);
        }
        const char* UserKey() const
        {
            return kstart_;
        }
        LookupKey(const std::string &userkey, uint64_t seqno)
        {

            //start_ = space_
            //make base128 varint for the length of the userkey
            //int keylen = size of varint
            //int endd = keylen + decoded varint + 8
            //if endd > 200
                //start_ = new char[endd];
                //kstart_ = (start_ + size in bytes of the varint)
                //end_ = kstart_ + decoded_varint + 8;
                //trailer = (seqno << 8) | 0x01;
                //use memcpy to write the varint, userkey, trailer to the new char[];
                //return;

            //kstart_ = (start_ + size in bytes of the varint)
            //end_ = kstart_ + decoded_varint + 8;
            //trailer = (seqno << 8) | 0x01;
            //use memcpy to write the varint, userkey, trailer into space_ 

        }
        ~LookupKey()
        {
            // if(start_ != space_) delete[] start_;
        }
        LookupKey(const LookupKey&) = delete;
        LookupKey& operator=(const LookupKey&) = delete;
    };
    void Add(uint64_t sequence, ValueType type, const std::string& key, const std::string& value);
    bool Get(const LookupKey& key, std::string* value, Status* status);
    size_t ApproximateMemoryUsage() const;

    memtable(const memtable&) = delete;
    memtable& operator=(const memtable&) = delete;
    memtable(memtable&&) = delete;
    memtable& operator=(memtable&&) = delete;

    private:
    struct KeyComparator
    {int operator()(const char* rec1, const char* rec2)const;};
    Arena arena;
    SkipList<const char*, KeyComparator> skiplist;
    std::atomic<size_t> memory_usage;

};