#include "11.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

namespace aoc11
{
namespace
{
using T = uint64_t;

struct Monkey
{
    using Op = std::function<T(T)>;

    T id;
    std::vector<T> items;
    Op op, test;
    size_t opCount = 0;
    T divisor;
};

std::istream& operator>>(std::istream& is, Monkey& monkey)
{
    static constexpr char sep = ',';
    static constexpr char add = '+';
    static constexpr char mul = '*';

    char c;
    std::string token;
    T value;

    is >> token;
    if (is.eof())
    {
        return is;
    }
    is >> monkey.id >> c;
    is >> token >> token;

    while (1)
    {
        is >> value;
        monkey.items.push_back(value);
        is >> c;
        if (c != sep)
        {
            break;
        }
    }

    is >> token >> token >> token >> token >> c >> token;

    if (token != "old")
    {
        value = std::stoul(token);
    }

    switch (c)
    {
    case add:
    {
        if (token == "old")
        {
            monkey.op = [](const T input){ return input + input; };
        }
        else
        {
            monkey.op = [value](const T input){ return input + value; };
        }
        break;
    }
    case mul:
    {
        if (token == "old")
        {
            monkey.op = [](const T input){ return input * input; };
        }
        else
        {
            monkey.op = [value](const T input){ return input * value; };
        }
        break;
    }
    default: throw std::runtime_error("Unknown operation: '" + std::string(1, c) + "'");
    }

    T divisor, trueResult, falseResult;
    is >> token >> token >> token >> divisor;
    is >> token >> token >> token >> token >> token >> trueResult;
    is >> token >> token >> token >> token >> token >> falseResult;

    monkey.divisor = divisor;

    monkey.test = [divisor, trueResult, falseResult](const T value)
    {
        return value % divisor == 0 ? trueResult : falseResult;
        std::cout << "  Test: " << value << " % " << divisor << " == ";
        if (value % divisor == 0)
        {
            std::cout << "true, returning: " << trueResult << std::endl;
            return trueResult;
        }
        std::cout << "false, returning: " << falseResult << std::endl;
        return falseResult;
    };

    return is;
}

using Monkeys = std::vector<Monkey>;

Monkeys parse(std::istream& is)
{
    Monkeys monkeys;

    while (1)
    {
        Monkey monkey;

        is >> monkey;
        if (is.eof())
        {
            break;
        }
        monkeys.push_back(monkey);
    }

    return monkeys;
}

void testMonkey(const Monkey& monkey, const T id, const std::vector<T>& items, const T op, const T opResult, const T test, const T testResult)
{
    REQUIRE(monkey.id == id);
    REQUIRE(monkey.items == items);
    REQUIRE(monkey.op(op) == opResult);
    REQUIRE(monkey.test(test) == testResult);
}

void exec(Monkeys& monkeys, const size_t rounds, const size_t worryLevelFix)
{
    T mod = 1;
    for (const auto& monkey : monkeys)
    {
        mod *= monkey.divisor;
    }

    for (size_t i = 0; i < rounds; ++i)
    {
        for (auto& monkey : monkeys)
        {
            for (const auto item : monkey.items)
            {
                const auto worryLevel = (monkey.op(item) / worryLevelFix) % mod;
                const size_t nextMonkey = monkey.test(worryLevel);

                monkeys[nextMonkey].items.push_back(worryLevel);
                ++monkey.opCount;
            }
            monkey.items.clear();
        }
    }
}

T solve(Monkeys monkeys, const size_t rounds, const size_t worryLevelDiv)
{
    exec(monkeys, rounds, worryLevelDiv);

    size_t max1 = 0, max2 = 0;
    for (const auto& monkey : monkeys)
    {
        max2 = std::max(monkey.opCount, max2);
        if (max2 > max1)
        {
            std::swap(max1, max2);
        }
    }

    return max1 * max2;
}

T solvePart1(const Monkeys& monkeys)
{
    return solve(monkeys, 20, 3);
}

T solvePart2(const Monkeys& monkeys)
{
    return solve(monkeys, 10000, 1);
}

}

TEST_CASE("2022-day-11")
{
    std::stringstream ss;
    ss << "Monkey 0:" << std::endl
       << "  Starting items: 79, 98" << std::endl
       << "  Operation: new = old * 19" << std::endl
       << "  Test: divisible by 23" << std::endl
       << "    If true: throw to monkey 2" << std::endl
       << "    If false: throw to monkey 3" << std::endl;

    Monkey monkey;
    ss >> monkey;

    testMonkey(monkey, 0, {79, 98}, 2, 38, 23, 2);

    auto input = Utility::getTestData("11/test-input.txt");

    const auto monkeys = parse(input);

    REQUIRE(monkeys.size() == 4);
    testMonkey(monkeys[0], 0, {79, 98}, 2, 38, 23, 2);
    testMonkey(monkeys[1], 1, {54, 65, 75, 74}, 6, 12, 38, 2);
    testMonkey(monkeys[2], 2, {79, 60, 97}, 2, 4, 26, 1);
    testMonkey(monkeys[3], 3, {74}, 2, 5, 17, 0);

    REQUIRE(solvePart1(monkeys) == 10605);
    REQUIRE(solvePart2(monkeys) == 2713310158);
}

void solution()
{
    auto monkeys = parse(std::cin);
    std::cout << solvePart1(monkeys) << std::endl;
    std::cout << solvePart2(monkeys) << std::endl;
}
}
