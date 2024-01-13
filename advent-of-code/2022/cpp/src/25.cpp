#include "25.hpp"

#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <sstream>
#include <vector>

namespace aoc25
{
namespace
{

constexpr std::array<std::pair<char, Utility::T>, 5> snafu = {std::make_pair('0', 0),
                                                           std::make_pair('1', 1),
                                                           std::make_pair('2', 2),
                                                           std::make_pair('=', -2),
                                                           std::make_pair('-', -1)};

constexpr Utility::T fromSnafuChar(const char c)
{
    for (const auto& [snafuChar, val] : snafu)
    {
        if (c == snafuChar)
        {
            return val;
        }
    }
    throw std::runtime_error("Unknown SNAFU char: '" + std::string(1, c) + "'");
}

Utility::T fromSnafu(const std::string& str)
{
    Utility::T order = 1, result = 0;
    for (auto it = std::rbegin(str); it != std::rend(str); ++it)
    {
        result += order * fromSnafuChar(*it);
        order *= 5;
    }
    return result;
}

std::string toSnafu(Utility::T val)
{
    if (val == 0)
    {
        return "0";
    }

    std::string str;

    while (val)
    {
        str.push_back(snafu[val % 5].first);
        val = (val + 2) / 5;
    }

    std::reverse(std::begin(str), std::end(str));

    return str;
}

using Data = std::vector<std::string>;

Data parse(std::istream& is)
{
    Data data;
    std::string s;
    while (is >> s)
    {
        data.push_back(s);
    }
    return data;
}

std::string solve(const Data& data)
{
    Utility::T result = 0;
    for (const auto& snafu : data)
    {
        result += fromSnafu(snafu);
    }
    return toSnafu(result);
}
}

TEST_CASE("2022-day-25")
{
    struct TestCase
    {
        std::string snafu;
        Utility::T value;
    };

    const auto testCase = GENERATE(TestCase{"0", 0},
                                   TestCase{"1", 1},
                                   TestCase{"2", 2},
                                   TestCase{"1=", 3},
                                   TestCase{"1-", 4},
                                   TestCase{"10", 5},
                                   TestCase{"11", 6},
                                   TestCase{"12", 7},
                                   TestCase{"2=", 8},
                                   TestCase{"2-", 9},
                                   TestCase{"20", 10},
                                   TestCase{"1=0", 15},
                                   TestCase{"1-0", 20},
                                   TestCase{"1=11-2", 2022},
                                   TestCase{"1121-1110-1=0", 314159265});

    REQUIRE(fromSnafu(testCase.snafu) == testCase.value);
    REQUIRE(toSnafu(testCase.value) == testCase.snafu);

    auto input = Utility::getTestData("25/sample.txt");
    const auto data = parse(input);
    REQUIRE(solve(data) == "2=-1=0");
}

void solution()
{
    const auto data = parse(std::cin);
    std::cout << solve(data) << std::endl;
}
}
