#include "03.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string_view>
#include <vector>

namespace aoc03
{
namespace
{
using T = uint64_t;

struct Rucksack
{
    std::unique_ptr<std::string> content;
    std::string_view firstHalf, secondHalf;
};

using Data = std::vector<Rucksack>;

std::optional<char> intersection(const std::vector<Utility::CharMap>& maps)
{
    for (char c : {'a', 'A'})
    {
        for (size_t i = 0; i < 26; ++i, ++c)
        {
            bool allContainChar = true;
            for (const auto& map : maps)
            {
                allContainChar = allContainChar && map.Contains(c);
            }
            if (allContainChar)
            {
                return c;
            }
        }
    }
    return std::nullopt;
}

Data parse(std::istream& is)
{
    Data data;

    std::string line;
    while (1)
    {
        is >> line;

        if (is.eof())
        {
            break;
        }

        Rucksack rucksack;

        rucksack.content = std::make_unique<std::string>(std::move(line));

        const auto half = rucksack.content->size() / 2;
        rucksack.firstHalf = std::string_view(rucksack.content->data(), half);
        rucksack.secondHalf = std::string_view(rucksack.content->data() + half, half);

        data.push_back(std::move(rucksack));
    }

    return data;
}

T solvePart1(const Data& data)
{
    T prioritySum = 0;

    for (const auto& rucksack : data)
    {
        if (const auto common = intersection({Utility::CharMap(rucksack.firstHalf), Utility::CharMap(rucksack.secondHalf)}); common.has_value())
        {
            prioritySum += Utility::CharMap::charToVal(common.value());
        }
    }

    return prioritySum;
}

T solvePart2(const Data& data)
{
    T prioritySum = 0;

    for (size_t i = 0; i < data.size(); i += 3)
    {
        if (const auto common = intersection({Utility::CharMap(*data[i].content),
                                              Utility::CharMap(*data[i + 1].content),
                                              Utility::CharMap(*data[i + 2].content)}); common.has_value())
        {
            prioritySum += Utility::CharMap::charToVal(common.value());
        }
    }

    return prioritySum;
}
}

TEST_CASE("2022-day-03")
{
    std::stringstream ss;
    ss << R"(vJrwpWtwJgWrhcsFMMfFFhFp
          jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
          PmmdzqPrVvPwwTWBwg
          wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
          ttgJtRGJQctTZtZT
          CrZsJsPPZsGzwwsLwLmpwMDw)" << std::endl;

    const auto data = parse(ss);

    REQUIRE(data.size() == 6);
    REQUIRE(*data[0].content == "vJrwpWtwJgWrhcsFMMfFFhFp");
    REQUIRE(*data[1].content == "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL");
    REQUIRE(*data[2].content == "PmmdzqPrVvPwwTWBwg");
    REQUIRE(*data[3].content == "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn");
    REQUIRE(*data[4].content == "ttgJtRGJQctTZtZT");
    REQUIRE(*data[5].content == "CrZsJsPPZsGzwwsLwLmpwMDw");

    REQUIRE(data[0].firstHalf  == "vJrwpWtwJgWr");
    REQUIRE(data[0].secondHalf == "hcsFMMfFFhFp");
    REQUIRE(data[1].firstHalf  == "jqHRNqRjqzjGDLGL");
    REQUIRE(data[1].secondHalf == "rsFMfFZSrLrFZsSL");
    REQUIRE(data[2].firstHalf  == "PmmdzqPrV");
    REQUIRE(data[2].secondHalf == "vPwwTWBwg");
    REQUIRE(data[3].firstHalf  == "wMqvLMZHhHMvwLH");
    REQUIRE(data[3].secondHalf == "jbvcjnnSBnvTQFn");
    REQUIRE(data[4].firstHalf  == "ttgJtRGJ");
    REQUIRE(data[4].secondHalf == "QctTZtZT");
    REQUIRE(data[5].firstHalf  == "CrZsJsPPZsGz");
    REQUIRE(data[5].secondHalf == "wwsLwLmpwMDw");

    REQUIRE(solvePart1(data) == 157);
    REQUIRE(solvePart2(data) == 70);
}

void solution()
{
    const auto data = parse(std::cin);
    std::cout << solvePart1(data) << std::endl;
    std::cout << solvePart2(data) << std::endl;
}
}
