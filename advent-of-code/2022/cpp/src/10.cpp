#include "10.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <variant>
#include <vector>

namespace aoc10
{
namespace
{
struct Noop
{};

using T = int64_t;
using Instruction = std::variant<T, Noop>;
using Instructions = std::vector<Instruction>;

static constexpr std::string_view AddInstr = "addx";
static constexpr std::string_view NoopInstr = "noop";

Instructions parse(std::istream& is)
{
    Instructions instructions;

    std::string instr;
    T val;
    while (1)
    {
        is >> instr;
        if (is.eof())
        {
            break;
        }
        if (instr == AddInstr)
        {
            is >> val;
            instructions.push_back(val);
        }
        else if (instr == NoopInstr)
        {
            instructions.push_back(Noop{});
        }
    }

    return instructions;
}

class Hook
{
public:
    void CheckAndSet(const size_t cycle, const T reg);
    T GetCount() const;

private:
    T counter = 0;
};

void Hook::CheckAndSet(const size_t cycle, const T reg)
{
    if (cycle >= 20 && (cycle - 20) % 40 == 0)
    {
        counter += reg * cycle;
    }
}

T Hook::GetCount() const
{
    return counter;
}

class Display
{
public:
    using Data = std::vector<std::string>;

    void DrawPixel(const T x);
    Data Dump() const;

private:
    static constexpr T width = 40;
    static constexpr T height = 6;
    bool data[height][width] = {};

    T row = 0, col = 0;
};

void Display::DrawPixel(const T x)
{
    if (col >= x - 1 && col <= x + 1)
    {
        data[row][col] = true;
    }

    ++col;
    if (col >= width)
    {
        col = 0;
        ++row;
        if (row >= height)
        {
            row = 0;
        }
    }
}

Display::Data Display::Dump() const
{
    Data dumpedData;

    std::string rowStr;
    for (size_t i = 0; i < height; ++i)
    {
        rowStr.clear();
        for (size_t j = 0; j < width; ++j)
        {
            rowStr.push_back(data[i][j] ? '#' : '.');
        }
        dumpedData.push_back(rowStr);
    }

    return dumpedData;
}

std::ostream& operator<<(std::ostream& os, const Display::Data& data)
{
    for (const auto& row : data)
    {
        os << row << std::endl;
    }
    return os;
}

template <typename HookT>
class Exec
{
public:
    Exec(HookT& hook, Display& display)
        : hook(hook)
        , display(display)
    {}

    void Execute(const Instructions& instructions)
    {
        reg = 1;
        cycle = 0;

        hook.CheckAndSet(cycle, reg);
        for (const auto& instruction : instructions)
        {
            ++cycle;
            hook.CheckAndSet(cycle, reg);

            display.DrawPixel(reg);

            if (std::holds_alternative<T>(instruction))
            {
                ++cycle;
                hook.CheckAndSet(cycle, reg);

                display.DrawPixel(reg);

                reg += std::get<T>(instruction);
            }
        }
    }

private:
    T reg = 1;
    size_t cycle = 0;
    HookT& hook;
    Display& display;
};

std::pair<T, Display::Data> solve(const Instructions& instructions)
{
    Hook hook;
    Display display;
    Exec exec(hook, display);

    exec.Execute(instructions);

    return {hook.GetCount(), display.Dump()};
}

T solvePart1(const Instructions& instructions)
{
    return solve(instructions).first;
}

Display::Data solvePart2(const Instructions& instructions)
{
    return solve(instructions).second;
}
}

TEST_CASE("2022-day-10")
{
    std::stringstream ss;
    ss << "noop" << std::endl
       << "addx 3" << std::endl
       << "addx -5" << std::endl;

    const auto instructions = parse(ss);

    REQUIRE(instructions.size() == 3);
    REQUIRE(std::holds_alternative<Noop>(instructions[0]));
    REQUIRE(std::get<T>(instructions[1]) == 3);
    REQUIRE(std::get<T>(instructions[2]) == -5);

    auto input = Utility::getTestData("10/test-input.txt");

    const auto testInstructions = parse(input);
    const auto result = solvePart1(testInstructions);
    REQUIRE(result == 13140);

    const auto displayData = solvePart2(testInstructions);

    Display::Data expected =
    {
        "##..##..##..##..##..##..##..##..##..##..",
        "###...###...###...###...###...###...###.",
        "####....####....####....####....####....",
        "#####.....#####.....#####.....#####.....",
        "######......######......######......####",
        "#######.......#######.......#######....."
    };

    REQUIRE(displayData == expected);
}

void solution()
{
    const auto instructions = parse(std::cin);
    std::cout << solvePart1(instructions) << std::endl;
    std::cout << solvePart2(instructions) << std::endl;
}
}
