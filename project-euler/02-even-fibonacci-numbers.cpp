#include <iostream>

int main()
{
    int64_t f1 = 1, f2 = 2;
    const int64_t max = 4'000'000;
    int64_t sum = 0;

    while (f1 <= max) 
    {
        if (f2 % 2 == 0)
        {
            sum += f2;
        }
        const auto tmp = f2;
        f2 = f1 + f2;
        f1 = tmp;
    }

    std::cout << sum << std::endl;

    return 0;
}
