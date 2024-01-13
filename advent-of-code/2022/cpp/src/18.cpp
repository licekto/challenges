#include "18.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace aoc18
{
namespace
{
struct Bounds
{
    static constexpr int32_t MaxVal = 1000;
    Bounds();
    void addToBounds(const Utility::T x, const Utility::T y, const Utility::T z);
    bool isInBounds(const Utility::Coord& c) const;

    Utility::Coord min, max;
};

Bounds::Bounds()
    : min{MaxVal, MaxVal, MaxVal}
    , max{0, 0, 0}
{}

void Bounds::addToBounds(const Utility::T x, const Utility::T y, const Utility::T z)
{
    min.x = std::min(x, min.x);
    min.y = std::min(y, min.y);
    min.z = std::min(z, min.z);

    max.x = std::max(x, max.x);
    max.y = std::max(y, max.y);
    max.z = std::max(z, max.z);
}

bool Bounds::isInBounds(const Utility::Coord& c) const
{
    return c.x >= min.x && c.x <= max.x
        && c.y >= min.y && c.y <= max.y
        && c.z >= min.z && c.z <= max.z;
}

struct InputData
{
    Utility::CoordSet cubes;
    Bounds bounds;
};

InputData parse(std::istream& is)
{
    InputData inputData;

    char c;
    int32_t x, y, z;

    while (1)
    {
        is >> x;
        if (is.eof())
        {
            break;
        }
        is >> c;
        is >> y;
        is >> c;
        is >> z;

        inputData.bounds.addToBounds(x, y, z);

        inputData.cubes.insert({x, y, z});
    }

    --inputData.bounds.min.x;
    --inputData.bounds.min.y;
    --inputData.bounds.min.z;
    ++inputData.bounds.max.x;
    ++inputData.bounds.max.y;
    ++inputData.bounds.max.z;

    return inputData;
}

const std::vector<Utility::Coord> neighbors { {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };

Utility::CoordSet fillOutside(const Utility::CoordSet& cubes, const Bounds& bounds)
{
    const auto start = bounds.min;

    Utility::CoordSet outside;

    std::queue<Utility::Coord> q;
    q.push(start);

    while(!q.empty())
    {
        const auto current = q.front();
        q.pop();

        for (const auto& neighbor : neighbors)
        {
            const auto next = current + neighbor;
            if (outside.contains(next) || !bounds.isInBounds(next) || cubes.contains(next))
            {
                continue;
            }
            outside.insert(next);
            q.push(next);
        }
    }

    return outside;
}

using Condition = std::function<bool(const Utility::Coord&)>;

size_t solve(const Utility::CoordSet& cubes, Condition&& condition)
{
    size_t area = 0;

    for (const auto& cube : cubes)
    {
        for (const auto& neighbor : neighbors)
        {
            const auto next = cube + neighbor;
            if (std::forward<Condition>(condition)(next))
            {
                ++area;
            }
        }
    }

    return area;
}

size_t solvePart2(const Utility::CoordSet& cubes, const Bounds& bounds)
{
    const auto outside = fillOutside(cubes, bounds);
    return solve(cubes, [&outside](const Utility::Coord& c){ return outside.contains(c); });
}

size_t solvePart1(const Utility::CoordSet& cubes)
{
    return solve(cubes, [&cubes](const Utility::Coord& c){ return !cubes.contains(c); });
}
}

TEST_CASE("2022-day-18")
{
    auto input = Utility::getTestData("18/test-input.txt");
    const auto inputData = parse(input);

    REQUIRE(inputData.cubes.size() == 13);
    REQUIRE(inputData.cubes.contains({2,2,2}));
    REQUIRE(inputData.cubes.contains({1,2,2}));
    REQUIRE(inputData.cubes.contains({3,2,2}));
    REQUIRE(inputData.cubes.contains({2,1,2}));
    REQUIRE(inputData.cubes.contains({2,3,2}));
    REQUIRE(inputData.cubes.contains({2,2,1}));
    REQUIRE(inputData.cubes.contains({2,2,3}));
    REQUIRE(inputData.cubes.contains({2,2,4}));
    REQUIRE(inputData.cubes.contains({2,2,6}));
    REQUIRE(inputData.cubes.contains({1,2,5}));
    REQUIRE(inputData.cubes.contains({3,2,5}));
    REQUIRE(inputData.cubes.contains({2,1,5}));
    REQUIRE(inputData.cubes.contains({2,3,5}));

    REQUIRE(solvePart1(inputData.cubes) == 64);
    REQUIRE(solvePart2(inputData.cubes, inputData.bounds) == 58);
}

void solution()
{
    const auto inputData = parse(std::cin);
    std::cout << solvePart1(inputData.cubes) << std::endl;
    std::cout << solvePart2(inputData.cubes, inputData.bounds) << std::endl;
}
}
