#include "12.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <iostream>
#include <limits>
#include <optional>
#include <queue>
#include <sstream>
#include <vector>

namespace aoc12
{
namespace
{
struct Map
{
    static constexpr char S = 'S';
    static constexpr char E = 'E';

    struct Item
    {
        char elevation;
        bool visited = false;
    };

    using Row = std::vector<Item>;
    using MapData = std::vector<Row>;

    Utility::Coord s, e;
    MapData data;

    Item& operator[](const Utility::Coord& coord);
    Item at(const Utility::Coord& coord) const;
    bool isValid(const Utility::Coord& current);
    void print() const;
};

Map::Item& Map::operator[](const Utility::Coord& coord)
{
    return data[coord.x][coord.y];
}

Map::Item Map::at(const Utility::Coord& coord) const
{
    return data[coord.x][coord.y];
}

bool Map::isValid(const Utility::Coord& next)
{
    return !(next.x < 0 || next.x >= static_cast<int64_t>(data.size()) || next.y < 0 || next.y >= static_cast<int64_t>(data[0].size()));
}

void Map::print() const
{
    for (const auto& row : data)
    {
        for (const auto& item : row)
        {
            if (item.visited)
            {
                std::cout << "-";
            }
            else
            {
                std::cout << item.elevation;
            }
        }
        std::cout << std::endl;
    }
}

Map parse(std::istream& is)
{
    Map map;

    std::string line;
    while (std::getline(is, line))
    {
        if (const auto pos = line.find_first_of(Map::S); pos != std::string::npos)
        {
            map.s = {static_cast<int64_t>(map.data.size()), static_cast<int64_t>(pos)};
            line[pos] = 'a';
        }
        if (const auto pos = line.find_first_of(Map::E); pos != std::string::npos)
        {
            map.e = {static_cast<int64_t>(map.data.size()), static_cast<int64_t>(pos)};
            line[pos] = 'z';
        }
        map.data.push_back({});
        for (const auto c : line)
        {
            map.data.back().push_back(Map::Item{c});
        }
    }

    return map;
}

Utility::T bfs(Map map, const Utility::Coord start)
{
    std::queue<std::pair<Utility::Coord, size_t>> q;
    map[start].visited = true;
    q.push({start, 0});

    while (!q.empty())
    {
        const auto current = q.front();
        q.pop();

        if (current.first == map.e)
        {
            return current.second;
        }

        std::vector<Utility::Coord> nexts{{current.first.x, current.first.y + 1}, {current.first.x, current.first.y - 1},
                                          {current.first.x + 1, current.first.y}, {current.first.x - 1, current.first.y}};

        for (auto& next : nexts)
        {
            if (!map.isValid(next) || map.at(next).visited)
            {
                continue;
            }
            const auto currentHeight = map.at(current.first).elevation;
            const auto nextHeight = map.at(next).elevation;
            if (currentHeight + 1 >= nextHeight)
            {
                map[next].visited = true;
                q.push({next, current.second + 1});
            }
        }
        //map.print();
    }

    return std::numeric_limits<Utility::T>::max();
}

Utility::T solvePart1(const Map& map)
{
    return bfs(map, map.s);
}

Utility::T solvePart2(const Map& map)
{
    Utility::T min = std::numeric_limits<Utility::T>::max();

    for (size_t i = 0; i < map.data.size(); ++i)
    {
        for (size_t j = 0; j < map.data[0].size(); ++j)
        {
            if (map.data[i][j].elevation == 'a')
            {
                const auto result = bfs(map, {static_cast<int64_t>(i), static_cast<int64_t>(j)});
                min = std::min(min, result);
            }
        }
    }

    return min;
}
}

TEST_CASE("2022-day-12")
{
    std::stringstream ss;
    ss << "Sabqponm" << std::endl
       << "abcryxxl" << std::endl
       << "accszExk" << std::endl
       << "acctuvwj" << std::endl
       << "abdefghi" << std::endl;

    const auto map = parse(ss);
    REQUIRE(map.data.size() == 5);
    REQUIRE(map.data[0].size() == 8);
    REQUIRE(map.data[4].size() == 8);
    REQUIRE(map.s.x == 0);
    REQUIRE(map.s.y == 0);
    REQUIRE(map.e.x == 2);
    REQUIRE(map.e.y == 5);

    REQUIRE(solvePart1(map) == 31);
}

void solution()
{
    const auto map = parse(std::cin);
    std::cout << solvePart1(map) << std::endl;
    std::cout << solvePart2(map) << std::endl;
}
}
