#include "24.hpp"

#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <queue>
#include <sstream>
#include <variant>
#include <vector>

namespace aoc24
{
namespace
{
struct Map
{
    static constexpr char Wall = '#';
    static constexpr char Empty = '.';
    static constexpr size_t MaxW = 122;
    static constexpr size_t MaxH = 27;
    static constexpr std::array<char, 4> dirs {'>', '<', 'v', '^'};

    Map execWinds();
    bool isWall(const Utility::Coord& c) const;
    bool isInBounds(const Utility::Coord& c) const;

    using Item = std::vector<char>;

    Utility::T w = 0, h = 0;
    Item data[MaxH][MaxW] = {};

private:
    void execWind(Map& newOne, const Utility::Coord& c);
    void clear();
};

Map Map::execWinds()
{
    Map newOne = *this;
    newOne.clear();

    for (Utility::T i = 0; i < h; ++i)
    {
        for (Utility::T j = 0; j < w; ++j)
        {
            const Utility::Coord c{j, i};
            if (isWall(c) || data[i][j].empty())
            {
                continue;
            }
            execWind(newOne, c);
        }
    }

    return newOne;
}

bool Map::isWall(const Utility::Coord& c) const
{
    return (c.y == 0 && c.x != 1) || (c.y == h - 1 && c.x != w - 2) || c.x == 0 || c.x == w - 1;
}

bool Map::isInBounds(const Utility::Coord& c) const
{
    return c.x >= 0 && c.x < w && c.y >= 0 && c.y < h;
}

void Map::execWind(Map& newOne, const Utility::Coord& c)
{
    const auto &winds = data[c.y][c.x];

    for (const auto wind : winds)
    {
        auto n = c;

        switch (wind)
        {
        case dirs[0] : ++n.x; break;
        case dirs[1] : --n.x; break;
        case dirs[2] : ++n.y; break;
        case dirs[3] : --n.y; break;
        }

        if (isWall(n))
        {
            switch (wind)
            {
            case dirs[0] : n.x = 1; break;
            case dirs[1] : n.x = w - 2; break;
            case dirs[2] : n.y = 1; break;
            case dirs[3] : n.y = h - 2; break;
            }
        }

        newOne.data[n.y][n.x].push_back(wind);
    }
}

void Map::clear()
{
    for (Utility::T i = 0; i < h; ++i)
    {
        for (Utility::T j = 0; j < w; ++j)
        {
            data[i][j].clear();
        }
    }
}

bool operator==(const Map& lhs, const Map& rhs)
{
    if (lhs.w != rhs.w || lhs.h != rhs.h)
    {
        return false;
    }

    for (Utility::T i = 0; i < lhs.h; ++i)
    {
        for (Utility::T j = 0; j < lhs.w; ++j)
        {
            if (lhs.data[i][j] != rhs.data[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
    for (Utility::T i = 0; i < map.h; ++i)
    {
        for (Utility::T j = 0; j < map.w; ++j)
        {
            if (map.isWall({j, i}))
            {
                os << Map::Wall;
                continue;
            }
            if (map.data[i][j].empty())
            {
                os << Map::Empty;
                continue;
            }

            const auto &item = map.data[i][j];
            if (item.size() == 1)
            {
                os << item[0];
            }
            else
            {
                os << item.size();
            }
        }
        os << std::endl;
    }
    return os;
}

Map parse(std::istream& is)
{
    Map map;

    std::string line;
    while (std::getline(is, line))
    {
        map.w = line.size();
        for (Utility::T i = 0; i < map.w; ++i)
        {
            if (line[i] != Map::Wall && line[i] != Map::Empty)
            {
                map.data[map.h][i].push_back(line[i]);
            }
        }
        ++map.h;
    }

    return map;
}

using MinuteMaps = std::vector<Map>;

MinuteMaps detectCycle(const Map& map)
{
    MinuteMaps maps;
    maps.push_back(map);

    do
    {
        maps.push_back(maps.back().execWinds());
    }
    while (maps.back() != maps.front());

    maps.pop_back();

    return maps;
}

Utility::Coord findWay(const MinuteMaps& maps, const Utility::Coord& start, const Utility::Coord& end)
{
    std::queue<Utility::Coord> q;
    Utility::CoordSet visited;

    std::vector<Utility::Coord> dirs{{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, 0}};

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        const auto currentState = q.front();
        q.pop();

        if (currentState.y == end.y && currentState.x == end.x)
        {
            return currentState;
        }

        auto& currentMap = maps[currentState.z % maps.size()];

        for (const auto& dir : dirs)
        {
            auto next = currentState + dir;
            ++next.z;
            auto hash = next;
            hash.z %= maps.size();

            if (currentMap.isInBounds(next) && !currentMap.isWall(next) && currentMap.data[next.y][next.x].empty() && !visited.contains(hash))
            {
                q.push(next);
                visited.insert(hash);
            }
        }
    }

    return {};
}

std::pair<size_t, size_t> solve(const MinuteMaps& maps)
{
    const Utility::Coord start{1, 0, 1}, end{maps[0].w - 2, maps[0].h - 1};
    const auto thereEnd = findWay(maps, start, end);
    const auto backEnd = findWay(maps, thereEnd, start);
    const auto thereAgainEnd = findWay(maps, backEnd, end);

    return {thereEnd.z - 1, thereAgainEnd.z - 1};
}
}

TEST_CASE("2022-day-24")
{
    auto input = Utility::getTestData("24/sample.txt");
    auto map = parse(input);

    auto input1 = Utility::getTestData("24/sample-step1.txt");
    auto map1 = parse(input1);

    std::stringstream ss1, ss2;
    ss1 << map1;
    ss2 << map.execWinds();
    REQUIRE(ss1.str() == ss2.str());

    const auto maps = detectCycle(map);

    REQUIRE(maps.size() == 12);

    const auto [there, back] = solve(maps);

    REQUIRE(there == 18);
    REQUIRE(back == 54);
}

void solution()
{
    const auto map = parse(std::cin);
    const auto maps = detectCycle(map);
    const auto [there, back] = solve(maps);
    std::cout << there << std::endl << back << std::endl;
}
}
