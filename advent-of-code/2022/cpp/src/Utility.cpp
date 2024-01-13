#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <stdexcept>

#include <Paths.hpp>

namespace Utility
{
uint64_t CharMap::charToVal(const char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 1;
    }
    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 1 + 26;
    }
    throw std::runtime_error("Unsupported characted '" + std::string(c, 1) + "'");
}

size_t CharMap::Size()
{
    return mapSize;
}

CharMap::CharMap(const std::string_view& str)
{
    for (const auto c : str)
    {
        map[charToVal(c)] = true;
    }
}

bool CharMap::Contains(const char c) const
{
    return map[charToVal(c)];
}

void CharMap::Clear()
{
    for (auto& item : map)
    {
        item = false;
    }
}

Coord::Coord(const T x, const T y, const T z)
    : x(x)
    , y(y)
    , z(z)
{}

bool operator==(const Coord& lhs, const Coord& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Coord& lhs, const Coord& rhs)
{
    return !(lhs == rhs);
}

Coord operator+(const Coord& lhs, const Coord& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

std::ostream& operator<<(std::ostream& os, const Coord& coord)
{
    return os << "[" << coord.x << "," << coord.y << "," << coord.z << "]";
}

uint64_t CoordHash::operator()(const Coord& coord) const
{
    std::string str;
    str.append(std::to_string(coord.x))
       .append(std::to_string(coord.y))
       .append(std::to_string(coord.z));
    return std::hash<std::string>{}(str);
}

size_t manhatanDistance(const Coord& c1, const Coord& c2)
{
    return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

void RangeSet::add(const T l, const T r)
{
    if (l > r)
    {
        throw std::runtime_error("l=" + std::to_string(l) + " > r=" + std::to_string(r));
    }

    auto after = upper_bound(l);

    RangeSet::iterator inserted;
    if (after == begin() || std::prev(after)->second < l)
    {
        auto [it, ins] = insert({l, r});
        inserted = it;
    }
    else if (std::prev(after)->second >= r)
    {
        return;
    }
    else
    {
        inserted = std::prev(after);
        inserted->second = r;
    }

    while (after != end() && r >= after->first)
    {
        inserted->second = std::max(r, after->second);
        after = erase(after);
    }

    if (after != end() && inserted->second + 1 == after->first)
    {
        inserted->second = after->second;
        erase(after);
    }

    if (inserted != begin() && std::prev(inserted)->second + 1 == inserted->first)
    {
        std::prev(inserted)->second = inserted->second;
        erase(inserted);
    }
}

std::ifstream getTestData(const std::string& filename)
{
    const std::string dataPath = std::string(Paths::testDataDir) + filename;

    std::ifstream input(dataPath);
    if (!input.is_open())
    {
        throw std::runtime_error("Wrong data path: '" + dataPath + "'");
    }

    return input;
}

TEST_CASE("2022-Utility")
{
    RangeSet rangeSet;

    rangeSet.add(5, 10);
    rangeSet.add(5, 10);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[5] == 10);

    rangeSet.add(2, 3);
    REQUIRE(rangeSet.size() == 2);
    REQUIRE(rangeSet[2] == 3);
    REQUIRE(rangeSet[5] == 10);

    rangeSet.add(12, 13);
    REQUIRE(rangeSet.size() == 3);
    REQUIRE(rangeSet[2] == 3);
    REQUIRE(rangeSet[5] == 10);
    REQUIRE(rangeSet[12] == 13);

    rangeSet.add(2, 7);
    REQUIRE(rangeSet.size() == 2);
    REQUIRE(rangeSet[2] == 10);
    REQUIRE(rangeSet[12] == 13);

    rangeSet.add(12, 14);
    REQUIRE(rangeSet.size() == 2);
    REQUIRE(rangeSet[2] == 10);
    REQUIRE(rangeSet[12] == 14);

    rangeSet.add(2, 14);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[2] == 14);

    rangeSet.clear();

    rangeSet.add(12, 14);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[12] == 14);

    rangeSet.add(6, 10);
    REQUIRE(rangeSet.size() == 2);
    REQUIRE(rangeSet[12] == 14);
    REQUIRE(rangeSet[6] == 10);

    rangeSet.add(0, 12);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[0] == 14);

    rangeSet.clear();

    rangeSet.add(0, 18);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[0] == 18);

    rangeSet.add(14, 14);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[0] == 18);

    rangeSet.clear();

    rangeSet.add(0, 16);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[0] == 16);

    rangeSet.add(20, 20);
    REQUIRE(rangeSet.size() == 2);
    REQUIRE(rangeSet[0] == 16);
    REQUIRE(rangeSet[20] == 20);

    rangeSet.add(12, 20);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[0] == 20);

    rangeSet.clear();
    rangeSet.add(1, 2);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[1] == 2);

    rangeSet.add(3, 4);
    REQUIRE(rangeSet.size() == 1);
    REQUIRE(rangeSet[1] == 4);
}
}
