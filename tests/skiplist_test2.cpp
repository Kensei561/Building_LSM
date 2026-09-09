#include <iostream>
#include <cassert>
#include <string>
#include <thread>
#include "arena.h"
#include "skiplist.h"
using namespace std;

int main()
{
    Arena arena;
    SkipList<int, string> list(&arena);

    thread ta([&list]() {
    for (int i = 0; i < 10000; ++i) {
        list.Insert(1, "ThreadA");
    }
    });

    thread tb([&list]() {
    for (int i = 0; i < 10000; ++i) {
        list.Insert(1, "ThreadB");
    }
    });

    ta.join();
    tb.join();    
}