#include "15.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace aoc15
{
namespace
{
struct Sensor
{
    Utility::Coord sensor, beacon, bottomLeft, topRight;
    size_t range = 0;

    void calculateDistance();
};

void Sensor::calculateDistance()
{
    range = manhatanDistance(sensor, beacon);
    bottomLeft.x = sensor.x - range;
    bottomLeft.y = sensor.y - range;
    topRight.x = sensor.x + range;
    topRight.y = sensor.y + range;
}

using Sensors = std::vector<Sensor>;

Sensors parse(std::istream& is)
{
    Sensors sensors;

    Sensor sensor;
    std::string token;
    char c;
    while (1)
    {
        is >> token >> token >> c >> c;
        if (is.eof())
        {
            break;
        }
        is >> sensor.sensor.x;
        is >> token >> c >> c;
        is >> sensor.sensor.y;
        is >> token >> token >> token >> token >> token >> c >> c;
        is >> sensor.beacon.x;
        is >> token >> c >> c;
        is >> sensor.beacon.y;

        sensor.calculateDistance();

        sensors.push_back(sensor);
    }

    return sensors;
}

Utility::RangeSet solve(const Sensors& sensors, const Utility::T y)
{
    Utility::RangeSet rangeSet;

    for (const auto& sensor : sensors)
    {
        const auto diff = std::abs(y - sensor.sensor.y);
        if (static_cast<size_t>(diff) <= sensor.range)
        {
            const Utility::T halfLength = sensor.range - diff;

            auto startX = sensor.sensor.x - halfLength;
            auto endX = sensor.sensor.x + halfLength;
            rangeSet.add(startX, endX);
        }
    }

    return rangeSet;
}

size_t solvePart1(const Sensors& sensors, const Utility::T y)
{
    const auto rangeSet = solve(sensors, y);
    const auto it = std::begin(rangeSet);
    return it->second - it->first;
}

size_t solvePart2(const Sensors& sensors, const Utility::T limit)
{
    static constexpr Utility::T mul = 4000000;

    for (Utility::T i = 0; i < limit; ++i)
    {
        const auto rangeSet = solve(sensors, i);
        if (rangeSet.size() == 2)
        {
            return mul * (rangeSet.begin()->second + 1) + i;
        }
    }
    return 0;
}

void testSensor(const Sensor& sensor, const Utility::T sx, const Utility::T sy, const Utility::T bx, const Utility::T by, const size_t distance)
{
    REQUIRE(sensor.sensor.x == sx);
    REQUIRE(sensor.sensor.y == sy);
    REQUIRE(sensor.beacon.x == bx);
    REQUIRE(sensor.beacon.y == by);
    REQUIRE(sensor.range == distance);
}
}

TEST_CASE("2022-day-15")
{
    auto input = Utility::getTestData("15/test-input.txt");
    const auto sensors = parse(input);

    REQUIRE(sensors.size() == 14);
    testSensor(sensors[0], 2, 18, -2, 15, 7);
    testSensor(sensors[1], 9, 16, 10, 16, 1);
    testSensor(sensors[2], 13, 2, 15, 3, 3);
    testSensor(sensors[3], 12, 14, 10, 16, 4);
    testSensor(sensors[4], 10, 20, 10, 16, 4);
    testSensor(sensors[5], 14, 17, 10, 16, 5);
    testSensor(sensors[6], 8, 7, 2, 10, 9);
    testSensor(sensors[7], 2, 0, 2, 10, 10);
    testSensor(sensors[8], 0, 11, 2, 10, 3);
    testSensor(sensors[9], 20, 14, 25, 17, 8);
    testSensor(sensors[10], 17, 20, 21, 22, 6);
    testSensor(sensors[11], 16, 7, 15, 3, 5);
    testSensor(sensors[12], 14, 3, 15, 3, 1);
    testSensor(sensors[13], 20, 1, 15, 3, 7);

    REQUIRE(solvePart1(sensors, 10) == 26);
    REQUIRE(solvePart2(sensors, 20) == 56000011);
}

void solution()
{
    const auto sensors = parse(std::cin);
    std::cout << solvePart1(sensors, 2000000) << std::endl;
    std::cout << solvePart2(sensors, 4000000) << std::endl;
}
}
