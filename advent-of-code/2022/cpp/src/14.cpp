#include "14.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace aoc14
{
namespace
{
std::istream& operator>>(std::istream& is, Utility::Coord& coord)
{
    char c;

    is >> coord.x;
    is >> c;
    is >> coord.y;

    return is;
}

using Line = std::vector<Utility::Coord>;
using Lines = std::vector<Line>;

Lines parse(std::istream& is)
{
    Lines lines;

    Line line;
    std::string lineStr;
    std::stringstream ss;

    while (std::getline(is, lineStr))
    {
        ss.clear();
        ss << lineStr;

        line.clear();
        Utility::Coord coord;
        std::string arrow;
        while (1)
        {
            ss >> coord;
            ss >> arrow;
            line.push_back(coord);
            if (ss.eof())
            {
                break;
            }
        }
        lines.push_back(line);
    }

    return lines;
}

class Map
{
public:
    enum class Item {Rock, Sand};
    using Points = std::unordered_map<Utility::Coord, Item, Utility::CoordHash>;

    Map(const Lines& lines);
    const Points& getPoints() const;
    Utility::T getLowerBound() const;
    std::pair<Utility::Coord, Utility::Coord> getLowestLine() const;
    void setLowestLine(const Utility::Coord& c1, const Utility::Coord& c2);
    bool dropSandAndCheckRested();
    void print() const;

private:
    void drawLine(Utility::Coord c1, Utility::Coord c2);
    bool isTaken(const Utility::Coord& coord) const;
    void fill(const Utility::Coord& coord);
    void calculateLowerBound(const Lines& lines);

    Utility::Coord start{500, 0};
    Utility::T lowerBound = 0;
    Utility::Coord lowestLeft, lowestRight;
    Points takenPoints;
};

Map::Map(const Lines& lines)
{
    for (const auto& line : lines)
    {
        for (size_t i = 1; i < line.size(); ++i)
        {
            drawLine(line[i - 1], line[i]);
        }
    }

    calculateLowerBound(lines);
}

const Map::Points& Map::getPoints() const
{
    return takenPoints;
}

void Map::drawLine(Utility::Coord c1, Utility::Coord c2)
{
    if (c1.x == c2.x)
    {
        if (c1.y > c2.y)
        {
            std::swap(c1.y, c2.y);
        }
        for (Utility::T i = c1.y; i <= c2.y; ++i)
        {
            takenPoints[Utility::Coord{c1.x, i}] = Item::Rock;
        }
    }
    else
    {
        if (c1.x > c2.x)
        {
            std::swap(c1.x, c2.x);
        }
        for (Utility::T i = c1.x; i <= c2.x; ++i)
        {
            takenPoints[Utility::Coord{i, c1.y}] = Item::Rock;
        }
    }
}

bool Map::isTaken(const Utility::Coord& coord) const
{
    return takenPoints.count(coord) == 1 || (coord.y == lowerBound && coord.x >= lowestLeft.x && coord.x <= lowestRight.x);
}

void Map::fill(const Utility::Coord& coord)
{
    takenPoints[coord] = Item::Sand;
}

void Map::calculateLowerBound(const Lines& lines)
{
    for (const auto& line : lines)
    {
        for (size_t i = 1; i < line.size(); ++i)
        {
            if (line[i - 1].y == line[i].y)
            {
                lowerBound = std::max(lowerBound, line[i].y);
                lowestLeft.x = std::min(line[i - 1].x, line[i].x);
                lowestLeft.y = line[i].y;

                lowestRight.x = std::max(line[i - 1].x, line[i].x);
                lowestRight.y = line[i].y;
            }
        }
    }
}

Utility::T Map::getLowerBound() const
{
    return lowerBound;
}

std::pair<Utility::Coord, Utility::Coord> Map::getLowestLine() const
{
    return {lowestLeft, lowestRight};
}

void Map::setLowestLine(const Utility::Coord& c1, const Utility::Coord& c2)
{
    lowerBound = c1.y;
    lowestLeft = c1;
    lowestRight = c2;
}

bool Map::dropSandAndCheckRested()
{
    if (isTaken(start))
    {
        return false;
    }
    Utility::Coord it = start;

    while (1)
    {
        while (!isTaken({it.x, it.y + 1}))
        {
            ++it.y;
            if (it.y > lowerBound)
            {
                return false;
            }
        }

        const Utility::Coord left{it.x - 1, it.y + 1};
        if (!isTaken(left))
        {
            it = left;
            continue;
        }

        const Utility::Coord right{it.x + 1, it.y + 1};
        if (!isTaken(right))
        {
            it = right;
            continue;
        }

        fill(it);
        return true;
    }
}

void Map::print() const
{
    Utility::T minx = 485, maxx = 510, maxy = 12;

    for (Utility::T i = 0; i <= maxy; ++i)
    {
        for (Utility::T j = minx; j <= maxx; ++j)
        {
            char c = '.';
            if (const auto it = takenPoints.find(Utility::Coord{j, i});
                it != std::end(takenPoints))
            {
                switch (it->second)
                {
                case Item::Rock: c = '#'; break;
                case Item::Sand: c = 'o'; break;
                }
            }

            if (j == start.x && i == start.y)
            {
                c = '+';
            }

            std::cout << c;
        }
        std::cout << std::endl;
    }
}

size_t solvePart1(Map map)
{
    size_t count = 0;

    while (map.dropSandAndCheckRested())
    {
        ++count;
    }

    return count;
}

size_t solvePart2(Map map)
{
    size_t count = 0;

    const auto min = std::numeric_limits<Utility::T>::min();
    const auto max = std::numeric_limits<Utility::T>::max();
    const auto currentLowerBound = map.getLowerBound();

    map.setLowestLine({min, currentLowerBound + 2}, {max, currentLowerBound + 2});
    while (map.dropSandAndCheckRested())
    {
        ++count;
    }

    return count;
}
}

TEST_CASE("2022-day-14")
{
    std::stringstream ss;
    ss << "498,4 -> 498,6 -> 496,6" << std::endl
       << "503,4 -> 502,4 -> 502,9 -> 494,9" << std::endl;

    const auto lines = parse(ss);
    REQUIRE(lines.size() == 2);

    REQUIRE(lines[0].size() == 3);
    REQUIRE(lines[0][0].x == 498);
    REQUIRE(lines[0][0].y == 4);
    REQUIRE(lines[0][1].x == 498);
    REQUIRE(lines[0][1].y == 6);
    REQUIRE(lines[0][2].x == 496);
    REQUIRE(lines[0][2].y == 6);

    REQUIRE(lines[1].size() == 4);
    REQUIRE(lines[1][0].x == 503);
    REQUIRE(lines[1][0].y == 4);
    REQUIRE(lines[1][1].x == 502);
    REQUIRE(lines[1][1].y == 4);
    REQUIRE(lines[1][2].x == 502);
    REQUIRE(lines[1][2].y == 9);
    REQUIRE(lines[1][3].x == 494);
    REQUIRE(lines[1][3].y == 9);


    Map map(lines);
    const auto points = map.getPoints();

    REQUIRE(points.size() == 20);
    REQUIRE(points.count(Utility::Coord{498, 4}) == 1);
    REQUIRE(points.count(Utility::Coord{498, 5}) == 1);
    REQUIRE(points.count(Utility::Coord{498, 6}) == 1);
    REQUIRE(points.count(Utility::Coord{497, 6}) == 1);
    REQUIRE(points.count(Utility::Coord{496, 6}) == 1);

    REQUIRE(points.count(Utility::Coord{503, 4}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 4}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 5}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 6}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 7}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 8}) == 1);
    REQUIRE(points.count(Utility::Coord{502, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{501, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{500, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{499, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{498, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{497, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{496, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{495, 9}) == 1);
    REQUIRE(points.count(Utility::Coord{494, 9}) == 1);

    REQUIRE(points.count(Utility::Coord{0, 0}) == 0);

    REQUIRE(map.getLowerBound() == 9);

    const auto [ll, lr] = map.getLowestLine();
    REQUIRE(ll.x == 494);
    REQUIRE(ll.y == 9);
    REQUIRE(lr.x == 502);
    REQUIRE(lr.y == 9);

    REQUIRE(solvePart1(map) == 24);
    REQUIRE(solvePart2(map) == 93);
}

void solution()
{
    const auto lines = parse(std::cin);
    Map map(lines);

    std::cout << solvePart1(map) << std::endl;
    std::cout << solvePart2(map) << std::endl;
}
}
