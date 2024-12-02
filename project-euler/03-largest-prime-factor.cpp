#include "SimpleLib.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using T = uint64_t;

bool isPrime(const T n)
{
    if (n == 2 || n == 3)
    {
        return true;
    }

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }

    for (T i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }

    return true;
}

T largestPrimeFactor(const T target)
{
    const T sqRoot = std::sqrt(target);

    for (T i = sqRoot; i >= 1; --i)
    {
        if (target % i == 0 && isPrime(i))
        {
            return i;
        }
    }

    return 0;
}

int main()
{
    const T target1 = 13195;
    const T target2 = 600851475143;

    ASSERT_EQ(largestPrimeFactor(target1), 29);
    ASSERT_EQ(largestPrimeFactor(target2), 6857);

    const auto stats = measure<std::chrono::nanoseconds>(10000, false, [target2](){ largestPrimeFactor(target2); });
    std::cout << calculateStats(stats).ToString("ns") << std::endl;

    return 0;
}
