#include "01.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

namespace aoc01
{
namespace
{
using T = int64_t;
using Data = std::vector<T>;

Data parse(std::istream &is)
{
    Data data;

    std::string line;
    T sum = 0;

    while (std::getline(is, line))
    {
        if (line.empty())
        {
            data.push_back(sum);
            sum = 0;
        }
        else
        {
            sum += std::stoul(line);
        }
    }
    data.push_back(sum);
    std::sort(std::begin(data), std::end(data), std::greater<T>());

    return data;
}

Data solve(const Data& data, const size_t nMax)
{
    Data result;

    for (size_t i = 0; i < std::min(nMax, data.size()); ++i)
    {
        result.push_back(data[i]);
    }

    return result;
}
}

TEST_CASE("2022-day-01")
{
    std::vector<std::vector<T>> data
    {
        {1, 2, 3},
        {4, 5, 6},
        {12, 13},
        {2},
        {1, 2}
    };

    std::stringstream ss;
    for (const auto& line : data)
    {
        for (const auto val : line)
        {
            ss << val << std::endl;
        }
        ss << std::endl;
    }

    const auto input = parse(ss);

    {
        const auto result = solve(input, 1);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 25);
    }

    {
        const auto result = solve(input, 3);
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == 25);
        REQUIRE(result[1] == 15);
        REQUIRE(result[2] == 6);
    }
}

void solution()
{
    const auto data = parse(std::cin);
    const auto result = solve(data, 3);
    T sum = 0;

    for (const auto max : result)
    {
        sum += max;
    }

    std::cout << result[0] << std::endl;
    std::cout << sum << std::endl;
}
}
