#include "05.hpp"

#include <catch2/catch_all.hpp>

#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

namespace aoc05
{
namespace
{
using T = char;
using Stacks = std::vector<std::stack<T>>;

using StackLine = std::vector<std::pair<size_t, char>>;

struct Action
{
    size_t quantity, from, to;
};

using Actions = std::vector<Action>;

StackLine parseLine(const std::string& line)
{
    StackLine stackLine;

    size_t stackIdx = 0;
    for (size_t i = 0; i < line.size(); i += 4, ++stackIdx)
    {
        if (std::isspace(line[i]))
        {
            continue;
        }
        stackLine.push_back({stackIdx, line[i + 1]});
    }

    return stackLine;
}

Stacks parseStacks(std::istream& is)
{
    std::vector<StackLine> stackLines;

    std::string line;
    while (std::getline(is, line))
    {
        const auto c = line[1];
        if (c >= '1' && c <= '9')
        {
            break;
        }
        else
        {
            stackLines.push_back(parseLine(line));
        }
    }

    size_t max = 0;
    for (const char c : line)
    {
        if (std::isspace(c))
        {
            continue;
        }
        max = c - '0';
    }

    Stacks data(max, std::stack<T>());

    for (auto it = std::rbegin(stackLines); it != std::rend(stackLines); ++it)
    {
        for (const auto& [idx, c] : *it)
        {
            data[idx].push(c);
        }
    }

    return data;
}

std::string stacksTop(const Stacks& stacks)
{
    std::string result;

    for (const auto& stack : stacks)
    {
        result.push_back(stack.top());
    }

    return result;
}

Actions parseActions(std::istream& is)
{
    Actions actions;

    std::string token;
    Action action;
    while (1)
    {
        is >> token >> action.quantity >> token >> action.from >> token >> action.to;
        if (is.eof())
        {
            break;
        }
        actions.push_back(action);
    }

    return actions;
}

std::string solvePart1(Stacks stacks, const Actions& actions)
{
    for (const auto& action : actions)
    {
        for (size_t i = 0; i < action.quantity; ++i)
        {
            stacks[action.to - 1].push(stacks[action.from - 1].top());
            stacks[action.from - 1].pop();
        }
    }

    return stacksTop(stacks);
}

std::string solvePart2(Stacks stacks, const Actions& actions)
{
    for (const auto& action : actions)
    {
        std::vector<char> pack;
        for (size_t i = 0; i < action.quantity; ++i)
        {
            pack.push_back(stacks[action.from - 1].top());
            stacks[action.from - 1].pop();
        }

        for (auto it = std::rbegin(pack); it != std::rend(pack); ++it)
        {
            stacks[action.to - 1].push(*it);
        }
    }

    return stacksTop(stacks);
}
}

TEST_CASE("2022-day-05")
{
    std::stringstream ss;
    ss << "    [D]" << std::endl
       << "[N] [C]" << std::endl
       << "[Z] [M] [P]" << std::endl
       << " 1   2   3" << std::endl
       << std::endl
       << "move 1 from 2 to 1" << std::endl
       << "move 3 from 1 to 3" << std::endl
       << "move 2 from 2 to 1" << std::endl
       << "move 1 from 1 to 2" << std::endl;

    auto stacks = parseStacks(ss);
    const auto actions = parseActions(ss);

    REQUIRE(solvePart1(stacks, actions) == "CMZ");
    REQUIRE(solvePart2(stacks, actions) == "MCD");
}

void solution()
{
    auto stacks = parseStacks(std::cin);
    const auto actions = parseActions(std::cin);
    std::cout << solvePart1(stacks, actions) << std::endl;
    std::cout << solvePart2(stacks, actions) << std::endl;
}
}
