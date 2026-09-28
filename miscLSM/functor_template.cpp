#include <iostream>

struct MyComparator
{
    public:
    bool operator()(int a, int b) const
    {
        return a > b;
    }
};

template<typename DataType, typename ComparatorType>
class Container{};

class Wrapper
{
    private:
    Container <int, MyComparator> my_container;
};