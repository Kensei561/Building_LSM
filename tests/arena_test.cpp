#include <iostream>
#include <arena.h>
using namespace std;

int main()
{
    Arena arena;
    char* t1 = arena.Allocate(1);
    uint64_t* p = reinterpret_cast<uint64_t*>(arena.AllocateAligned(8));
    *p = 0xFFFFFFFFFFFFFFFF;
}