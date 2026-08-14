#include <iostream>
#include <cassert>
#include <string>
#include "arena.h"
#include "skiplist.h"

int main() {
    Arena arena;
    SkipList<int, std::string> list(&arena);

    // Test 1: Contains on empty list
    assert(!list.Contains(10));

    // Test 2: Insertions
    list.Insert(10, "ten");
    list.Insert(20, "twenty");
    list.Insert(5, "five");

    // Test 3: Lookups
    assert(list.Contains(10));
    assert(list.Contains(20));
    assert(list.Contains(5));
    assert(!list.Contains(15));

    // Test 4: Key update
    list.Insert(10, "TEN_UPDATED");
    assert(list.Contains(10));

    std::cout << "All SkipList tests passed successfully!" << std::endl;
    return 0;
}