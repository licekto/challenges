#include "06.hpp"

#include <catch2/catch_all.hpp>

#include <sstream>
#include <iostream>
#include <vector>

namespace aoc06
{
namespace
{
using T = size_t;

std::string parse(std::istream& is)
{
    std::string line;
    std::getline(is, line);
    return line;
}

bool containsUniqueChars(const std::string_view& str)
{
    static constexpr size_t mapSize = 26;
    bool map[mapSize] = {};

    for (const auto c : str)
    {
        if (map[c - 'a'])
        {
            return false;
        }
        map[c - 'a'] = true;
    }
    return true;
}

size_t findConsecutiveUnique(const std::string& str, const size_t uniqueCount)
{
    for (size_t i = uniqueCount; i < str.size(); ++i)
    {
        const auto view = std::string_view(str.data() + i - uniqueCount, uniqueCount);
        if (containsUniqueChars(view))
        {
            return i;
        }
    }
    throw std::runtime_error(std::to_string(uniqueCount) + " consecutive unique charactes have not been found in the input.");
}

size_t solvePart1(const std::string& str)
{
    return findConsecutiveUnique(str, 4);
}

size_t solvePart2(const std::string& str)
{
    return findConsecutiveUnique(str, 14);
}
}

TEST_CASE("2022-day-06")
{
    const std::string input1 = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
    const std::string input2 = "bvwbjplbgvbhsrlpgdmjqwftvncz";
    const std::string input3 = "nppdvjthqldpwncqszvftbrmjlhg";
    const std::string input4 = "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg";
    const std::string input5 = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw";

    REQUIRE(solvePart1(input1) == 7);
    REQUIRE(solvePart1(input2) == 5);
    REQUIRE(solvePart1(input3) == 6);
    REQUIRE(solvePart1(input4) == 10);
    REQUIRE(solvePart1(input5) == 11);

    REQUIRE(solvePart2(input1) == 19);
    REQUIRE(solvePart2(input2) == 23);
    REQUIRE(solvePart2(input3) == 23);
    REQUIRE(solvePart2(input4) == 29);
    REQUIRE(solvePart2(input5) == 26);
}

void solution()
{
    const auto input = parse(std::cin);
    std::cout << solvePart1(input) << std::endl;
    std::cout << solvePart2(input) << std::endl;
}
}
