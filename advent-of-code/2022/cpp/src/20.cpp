#include "20.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace aoc20
{
namespace
{
using T = int64_t;
using List = std::list<T>;
using It = List::iterator;

std::istream& operator>>(std::istream& is, List& list)
{
    int32_t n;
    while (is >> n)
    {
        list.push_back(n);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const List& list)
{
    bool first = true;
    for (const auto n : list)
    {
        if (!first)
        {
            os << ",";
        }
        first = false;
        os << n;
    }

    return os;
}

It move(List& list, It target)
{
    const auto val = *target;
    const size_t n = std::abs(*target) % (list.size() - 1);
    if (n == 0)
    {
        return target;
    }

    auto it = target;
    for (size_t i = 0; i < n; ++i)
    {
        if (val > 0)
        {
            ++it;
            if (it == std::end(list))
            {
                it = std::begin(list);
            }
        }
        else
        {
            if (it == std::begin(list))
            {
                it = std::end(list);
            }
            --it;
        }
    }

    list.erase(target);
    if (val > 0)
    {
        ++it;
        if (it == std::end(list))
        {
            it = std::begin(list);
        }
    }
    return list.insert(it, val);
}

It exec(List& list, const size_t repeat = 1)
{
    std::unordered_map<size_t, It> idxToIt;
    size_t idx = 0;
    for (auto it = std::begin(list); it != std::end(list); ++it)
    {
        idxToIt[idx++] = it;
    }

    It zero;
    for (size_t r = 0; r < repeat; ++r)
    {
        for (size_t i = 0; i < list.size(); ++i)
        {
            auto it = idxToIt[i];
            auto iit = move(list, it);
            if (*iit == 0)
            {
                zero = iit;
            }
            idxToIt[i] = iit;
        }
    }
    return zero;
}

void applyKey(List& list, const T key)
{
    for (auto& val : list)
    {
        val *= key;
    }
}

T getNth(List& list, It it, size_t n)
{
    n %= list.size();
    for (size_t i = 0; i < n; ++i)
    {
        ++it;
        if (it == std::end(list))
        {
            it = std::begin(list);
        }
    }
    return *it;
}

T solvePart1(List list)
{
    auto zero = exec(list);
    return getNth(list, zero, 1000) + getNth(list, zero, 2000) + getNth(list, zero, 3000);
}

T solvePart2(List list)
{
    static constexpr T key = 811589153;
    applyKey(list, key);
    auto zero = exec(list, 10);
    return getNth(list, zero, 1000) + getNth(list, zero, 2000) + getNth(list, zero, 3000);
}

}

TEST_CASE("2022-day-20")
{
    std::vector<T> data{1, 2, -3, 3, -2, 0, 4};
    std::stringstream ss;
    for (const auto val : data)
    {
        ss << val << std::endl;
    }

    List list;
    ss >> list;

    auto copy1 = list;

    auto zero1 = exec(copy1);

    REQUIRE(getNth(copy1, zero1, 1000) == 4);
    REQUIRE(getNth(copy1, zero1, 2000) == -3);
    REQUIRE(getNth(copy1, zero1, 3000) == 2);

    auto copy2 = list;

    static constexpr T key = 811589153;
    applyKey(copy2, key);

    auto zero2 = exec(copy2, 10);
    REQUIRE(getNth(copy2, zero2, 1000) == 811589153);
    REQUIRE(getNth(copy2, zero2, 2000) == 2434767459);
    REQUIRE(getNth(copy2, zero2, 3000) == -1623178306);
}

void solution()
{
    List list;
    std::cin >> list;
    std::cout << solvePart1(list) << std::endl;
    std::cout << solvePart2(list) << std::endl;
}
}
