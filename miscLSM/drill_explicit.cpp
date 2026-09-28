#include <iostream>
using namespace std;

class drill
{
    public:
    drill();
    ~drill();

    private:
    atomic<size_t> a;
    drill(const drill&) = delete;
    drill& operator=(const drill&) = delete;
};
