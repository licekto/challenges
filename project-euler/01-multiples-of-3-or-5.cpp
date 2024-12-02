#include <cstdint>
#include <iostream>

using T = uint64_t;

int main()
{
    const T target = 1000;
    T sum = 0;

    for (T i = 3; i < target; ++i)
    {
        if (i % 3 == 0 || i % 5 == 0)
        {
            sum += i;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}
