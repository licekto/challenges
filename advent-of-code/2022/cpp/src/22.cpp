#include "22.hpp"

#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <sstream>
#include <variant>
#include <vector>

namespace aoc22
{
namespace
{

enum class Direction : uint8_t { R = 0, D = 1, L = 2, U = 3 };

std::ostream& operator<<(std::ostream& os, const Direction& dir)
{
    switch(dir)
    {
    case Direction::R : os << "R"; break;
    case Direction::D : os << "D"; break;
    case Direction::L : os << "L"; break;
    case Direction::U : os << "U"; break;
    }

    return os;
}

struct Position
{
    Direction direction {};
    Utility::Coord coord {};
};

bool operator==(const Position& lhs, const Position& rhs)
{
    return lhs.direction == rhs.direction && lhs.coord == rhs.coord;
}

Direction& operator++(Direction& dir)
{
    dir = static_cast<Direction>((static_cast<uint8_t>(dir) + 1) % 4);
    return dir;
}

Direction& operator--(Direction& dir)
{
    const auto val = static_cast<uint8_t>(dir);
    if (val == 0)
    {
        dir = Direction::U;
    }
    else
    {
        dir = static_cast<Direction>(val - 1);
    }
    return dir;
}

struct Map
{
    static constexpr char Boundary = 0;
    static constexpr char Wall = '#';
    static constexpr size_t MaxW = 152;
    static constexpr size_t MaxH = 202;
    using Data = char[MaxH][MaxW];
    using RowBounds = int64_t[MaxH];
    using ColBounds = int64_t[MaxW];

    Map(const Data& data, const size_t w, const size_t h,
        const RowBounds& rowBeginsAt, const RowBounds& rowEndsAt,
        const ColBounds& colBeginsAt, const ColBounds& colEndsAt);

    const size_t w = 0, h = 0;
    const Data data {};
    const RowBounds rowBeginsAt = {}, rowEndsAt = {};
    const ColBounds colBeginsAt = {}, colEndsAt = {};

    char operator[](const Position& pos) const;
};

Map::Map(const Data& data, const size_t w, const size_t h,
         const RowBounds& rowBeginsAt, const RowBounds& rowEndsAt,
         const ColBounds& colBeginsAt, const ColBounds& colEndsAt)
    : w(w)
    , h(h)
{
    for (size_t r = 0; r < h; ++r)
    {
        for (size_t c = 0; c < w; ++c)
        {
            const_cast<Data&>(this->data)[r][c] = data[r][c];
        }
        const_cast<RowBounds&>(this->rowBeginsAt)[r] = rowBeginsAt[r];
        const_cast<RowBounds&>(this->rowEndsAt)[r] = rowEndsAt[r];
    }
    for (size_t c = 0; c < w; ++c)
    {
        const_cast<ColBounds&>(this->colBeginsAt)[c] = colBeginsAt[c];
        const_cast<ColBounds&>(this->colEndsAt)[c] = colEndsAt[c];
    }
}

char Map::operator[](const Position& pos) const
{
    return data[pos.coord.y][pos.coord.x];
}

std::ostream& operator<<(std::ostream& os, const Map& map)
{
    os << "   ";
    for (size_t i = 0; i <= map.w; ++i)
    {
        if (i >= 100)
        {
            os << std::to_string(i / 100);
        }
        else
        {
            os << " ";
        }
    }
    os << std::endl;
    os << "   ";
    for (size_t i = 0; i <= map.w; ++i)
    {
        if (i >= 10)
        {
            const auto idx = i % 100;
            os << std::to_string(idx / 10);
        }
        else
        {
            os << " ";
        }
    }
    os << std::endl;
    os << "   ";
    for (size_t i = 0; i <= map.w; ++i)
    {
        os << std::to_string(i % 10);
    }
    os << std::endl;

    for (size_t r = 0; r < map.h; ++r)
    {
        if (r < 10)
        {
            os << "  ";
        }
        else if (r < 100)
        {
            os << " ";
        }
        os << r;
        for (size_t c = 0; c < map.w; ++c)
        {
            const auto ch = map.data[r][c];
            os << (ch == 0 ? ' ' : ch);
        }
        os << std::endl;
    }
    return os;
}

using Move = std::variant<size_t, Direction>;

using Moves = std::vector<Move>;

std::pair<Map, Moves> parse(std::istream& is)
{
    std::string line;
    size_t w = 0, h = 0, row = 1;
    char data[Map::MaxH][Map::MaxW] = {};
    Map::RowBounds rowBeginsAt = {}, rowEndsAt = {};
    Map::ColBounds colBeginsAt = {}, colEndsAt = {};

    while (std::getline(is, line))
    {
        if (line.empty())
        {
            break;
        }
        w = std::max(w, line.size());

        for (size_t i = 0; i < line.size(); ++i)
        {
            const auto ch = line[i];
            if (!std::isspace(ch))
            {
                if (rowBeginsAt[row] == 0)
                {
                    rowBeginsAt[row] = i + 1;
                }
                if (colBeginsAt[i + 1] == 0)
                {
                    colBeginsAt[i + 1] = row;
                }
                colEndsAt[i + 1] = row;
            }
            data[row][i + 1] = std::isspace(ch) ? 0 : ch;
        }
        rowEndsAt[row] = line.size();

        ++row;
    }

    ++row;
    h = row;

    Moves moves;

    size_t steps;
    char r;

    while (1)
    {
        is >> steps;
        if (is.eof())
        {
            break;
        }
        moves.push_back(steps);

        is >> r;
        if (is.eof())
        {
            break;
        }
        moves.push_back(r == 'R' ? Direction::R : Direction::L);
    }

    return {Map(data, w + 2, h, rowBeginsAt, rowEndsAt, colBeginsAt, colEndsAt), moves};
}

struct DirItem
{
    Utility::Coord cameFrom, goingTo;
    Direction dir;
};

using BoundsMap = Utility::CoordMap<std::vector<DirItem>>;

class Exec
{
public:
    Exec(const Map& map, const Moves& moves, const BoundsMap& boundsMap);
    bool isEnd() const;
    Position step(const bool cube = false);

private:
    using It = Moves::const_iterator;

    Position getNext() const;

    const Map& map;
    const Moves& moves;
    const BoundsMap& boundsMap;
    It currentMove;

    Position position;
};

Exec::Exec(const Map& map, const Moves& moves, const BoundsMap& boundsMap)
    : map(map)
    , moves(moves)
    , boundsMap(boundsMap)
    , currentMove(std::begin(this->moves))
    , position{Direction::R, {map.rowBeginsAt[1], 1}}
{}

bool Exec::isEnd() const
{
    return currentMove == std::end(moves);
}

Position Exec::step(const bool cube)
{
    if (std::holds_alternative<size_t>(*currentMove))
    {
        const auto steps = std::get<size_t>(*currentMove);
        //std::cout << steps << std::endl;
        for (size_t i = 0; i < steps; ++i)
        {
            auto next = getNext();

            if (map[next] == Map::Boundary)
            {
                if (!cube)
                {
                    switch (position.direction)
                    {
                    case Direction::R : next = {next.direction, {map.rowBeginsAt[next.coord.y], next.coord.y}}; break;
                    case Direction::D : next = {next.direction, {next.coord.x, map.colBeginsAt[next.coord.x]}}; break;
                    case Direction::L : next = {next.direction, {map.rowEndsAt[next.coord.y], next.coord.y}}; break;
                    case Direction::U : next = {next.direction, {next.coord.x, map.colEndsAt[next.coord.x]}}; break;
                    }
                }
                else
                {
                    auto it = boundsMap.find(next.coord);
                    if (it == std::end(boundsMap))
                    {
                        throw std::runtime_error("Not in bounds: [" + std::to_string(next.coord.x) + "," + std::to_string(next.coord.y) + "]");
                    }

                    //std::cout << next.coord << " -> ";

                    for (const auto dirItem : it->second)
                    {
                        if (dirItem.cameFrom == position.coord)
                        {
                            next = {dirItem.dir, dirItem.goingTo};
                            //std::cout << next.coord << std::endl;
                            break;
                        }
                    }
                }
            }

            if (map[next] == Map::Wall)
            {
                break;
            }

            position = next;
        }
    }
    else
    {
        const auto dir = std::get<Direction>(*currentMove);
        //std::cout << dir << std::endl;
        if (dir == Direction::R)
        {
            ++position.direction;
        }
        else if (dir == Direction::L)
        {
            --position.direction;
        }
    }

    ++currentMove;
    return position;
}

Position Exec::getNext() const
{
    Position newOne = position;
    switch (newOne.direction)
    {
        case Direction::R : ++newOne.coord.x; break;
        case Direction::D : ++newOne.coord.y; break;
        case Direction::L : --newOne.coord.x; break;
        case Direction::U : --newOne.coord.y; break;
    }
    return newOne;
}

void addBound(BoundsMap& boundsMap, const Utility::Coord& bound, const Utility::Coord& from, const Utility::Coord& to, const Direction dir)
{
    boundsMap[bound].push_back({from, to, dir});
}

BoundsMap buildTestBounds()
{
    BoundsMap boundsMap;

    // Horizontal
    // 2222 top -> 5555 top
    int64_t x = 9, y = 0;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {4 - i, 5}, Direction::D);
    }

    // 5555 top -> 2222 top
    x = 1, y = 4;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {12 - i, 1}, Direction::D);
    }

    // 1111 top -> 2222 left
    x = 5, y = 4;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {9, 1 + i}, Direction::R);
    }

    // 5555 bottom -> 0000 bottom
    x = 1, y = 9;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {12 - i, 12}, Direction::U);
    }

    // 1111 bottom -> 0000 left
    x = 5, y = 9;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {9, 12 - i}, Direction::R);
    }

    // 3333 top -> 4444 -> right
    x = 13, y = 9;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {12, 8 - i}, Direction::L);
    }

    // 0000 bottom -> 5555 bottom
    x = 9, y = 13;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {4 - i, 8}, Direction::U);
    }

    // 3333 bottom -> 5555 left
    x = 13, y = 13;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {1, 8 - i}, Direction::R);
    }

    // Vertical
    // 2222 left -> 1111 top
    x = 8, y = 1;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {5 + i, 5}, Direction::D);
    }

    // 5555 left -> 3333 bottom
    x = 0, y = 5;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {16 - i, 12}, Direction::U);
    }

    // 0000 left -> 1111 bottom
    x = 8, y = 9;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {8 - i, 8}, Direction::U);
    }

    // 2222 right -> 3333 left
    x = 13, y = 1;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {16, 9 + i}, Direction::L);
    }

    // 4444 right -> 3333 top
    x = 13, y = 5;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {16 - i, 9}, Direction::D);
    }

    // 3333 right -> 2222 right
    x = 17, y = 9;
    for (int64_t i = 0; i < 4; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {12, 4 - i}, Direction::L);
    }

    return boundsMap;
}

BoundsMap buildLargeBounds()
{
    BoundsMap boundsMap;
    static constexpr int64_t faceSize = 50;

    // Horizontal, top
    // 1 top -> 5 left
    int64_t x = 51, y = 0;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {1, 151 + i}, Direction::R);
    }
    // 2 top -> 5 bottom
    x = 101, y = 0;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {1 + i, 200}, Direction::U);
    }
    // 0 top -> 4 left
    x = 1, y = 100;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y + 1}, {51, 51 + i}, Direction::R);
    }

    // Horizontal, bottom
    // 2 bottom -> 4 right
    x = 101, y = 51;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {100, 51 + i}, Direction::L);
    }
    // 3 bottom -> 5 right
    x = 51, y = 151;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {50, 151 + i}, Direction::L);
    }
    // 5 bottom -> 2 top
    x = 1, y = 201;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x + i, y}, {x + i, y - 1}, {101 + i, 1}, Direction::D);
    }

    // Vertical, left
    // 1 left -> 0 left
    x = 50, y = 1;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {1, 150 - i}, Direction::R);
    }
    // 4 left -> 0 top
    x = 50, y = 51;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {1 + i, 101}, Direction::D);
    }
    // 0 left -> 1 left
    x = 0, y = 101;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {51, 50 - i}, Direction::R);
    }
    // 5 left -> 1 top
    x = 0, y = 151;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x + 1, y + i}, {51 + i, 1}, Direction::D);
    }

    // Vertical, right
    // 2 right -> 3 right
    x = 151, y = 1;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {100, 150 - i}, Direction::L);
    }
    // 4 right -> 2 bottom
    x = 101, y = 51;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {101 + i, 50}, Direction::U);
    }
    // 3 right -> 2 right
    x = 101, y = 101;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {150, 50 - i}, Direction::L);
    }
    // 5 right -> 3 bottom
    x = 51, y = 151;
    for (int64_t i = 0; i < faceSize; ++i)
    {
        addBound(boundsMap, {x, y + i}, {x - 1, y + i}, {51 + i, 150}, Direction::U);
    }

    return boundsMap;
}

size_t solvePart1(const Map& map, const Moves& moves)
{
    BoundsMap boundsMap;

    Exec exec(map, moves, boundsMap);

    Position pos;
    while (!exec.isEnd())
    {
        pos = exec.step();
    }

    return 1000 * pos.coord.y + 4 * pos.coord.x + static_cast<size_t>(pos.direction);
}

size_t solvePart2(const Map& map, const Moves& moves, const BoundsMap& boundsMap)
{
    Exec exec(map, moves, boundsMap);

    Position pos;
    while (!exec.isEnd())
    {
        pos = exec.step(true);
    }

    return 1000 * pos.coord.y + 4 * pos.coord.x + static_cast<size_t>(pos.direction);
}

TEST_CASE("2022-day-22-part2-test")
{
    auto input = Utility::getTestData("22/test-input.txt");
    const auto [map, moves] = parse(input);

    const auto boundsMap = buildTestBounds();
    Exec exec(map, moves, boundsMap);

    // 10R5L5R10L4R5L5
    // 10
    REQUIRE(exec.step(true) == Position{Direction::R, {11, 1}});
    // R
    REQUIRE(exec.step(true) == Position{Direction::D, {11, 1}});
    // 5
    REQUIRE(exec.step(true) == Position{Direction::D, {11, 6}});
    // L
    REQUIRE(exec.step(true) == Position{Direction::R, {11, 6}});
    // 5
    REQUIRE(exec.step(true) == Position{Direction::D, {15, 11}});

    REQUIRE(solvePart2(map, moves, boundsMap) == 5031);
}

TEST_CASE("2022-day-22-part2-large")
{
    auto input = Utility::getTestData("22/input1.txt");
    const auto [map, moves] = parse(input);
    const auto boundsMap = buildLargeBounds();
    //std::cout << "-------------" << std::endl;
    //std::cout << map << std::endl;
    REQUIRE(solvePart2(map, moves, boundsMap) == 129339);
}

TEST_CASE("2022-day-22")
{
    auto input = Utility::getTestData("22/test-input.txt");
    const auto [map, moves] = parse(input);

    Exec exec(map, moves, {});

    // 10R5L5R10L4R5L5

    REQUIRE(!exec.isEnd());
    // 10
    REQUIRE(exec.step() == Position{Direction::R, {11, 1}});
    REQUIRE(!exec.isEnd());
    // R
    REQUIRE(exec.step() == Position{Direction::D, {11, 1}});
    REQUIRE(!exec.isEnd());
    // 5
    REQUIRE(exec.step() == Position{Direction::D, {11, 6}});
    REQUIRE(!exec.isEnd());
    // L
    REQUIRE(exec.step() == Position{Direction::R, {11, 6}});
    REQUIRE(!exec.isEnd());
    // 5
    REQUIRE(exec.step() == Position{Direction::R, {4, 6}});
    REQUIRE(!exec.isEnd());
    // R
    REQUIRE(exec.step() == Position{Direction::D, {4, 6}});
    REQUIRE(!exec.isEnd());
    // 10
    REQUIRE(exec.step() == Position{Direction::D, {4, 8}});
    REQUIRE(!exec.isEnd());

    // L
    REQUIRE(exec.step() == Position{Direction::R, {4, 8}});
    REQUIRE(!exec.isEnd());
    // 4
    REQUIRE(exec.step() == Position{Direction::R, {8, 8}});
    REQUIRE(!exec.isEnd());
    // R
    REQUIRE(exec.step() == Position{Direction::D, {8, 8}});
    REQUIRE(!exec.isEnd());
    // 5
    REQUIRE(exec.step() == Position{Direction::D, {8, 6}});
    REQUIRE(!exec.isEnd());
    // L
    REQUIRE(exec.step() == Position{Direction::R, {8, 6}});
    REQUIRE(!exec.isEnd());
    // 5
    REQUIRE(exec.step() == Position{Direction::R, {8, 6}});
    REQUIRE(exec.isEnd());

    REQUIRE(solvePart1(map, moves) == 6032);
}

bool areSame(const std::vector<char>& str, const char line[Map::MaxW])
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] != line[i])
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool isMove(const Move& move, const Move& expected)
{
    return std::holds_alternative<T>(move) && std::get<T>(move) == std::get<T>(expected);
}

}

TEST_CASE("2022-day-22-parse")
{
    auto input = Utility::getTestData("22/test-input.txt");
    const auto [map, moves] = parse(input);

    REQUIRE(map.w == 18);
    REQUIRE(map.h == 14);
    REQUIRE(map.rowBeginsAt[0] == 0);
    REQUIRE(map.rowBeginsAt[1] == 9);
    REQUIRE(map.rowBeginsAt[2] == 9);
    REQUIRE(map.rowBeginsAt[3] == 9);
    REQUIRE(map.rowBeginsAt[4] == 9);
    REQUIRE(map.rowBeginsAt[5] == 1);
    REQUIRE(map.rowBeginsAt[6] == 1);
    REQUIRE(map.rowBeginsAt[7] == 1);
    REQUIRE(map.rowBeginsAt[8] == 1);
    REQUIRE(map.rowBeginsAt[9] == 9);
    REQUIRE(map.rowBeginsAt[10] == 9);
    REQUIRE(map.rowBeginsAt[11] == 9);
    REQUIRE(map.rowBeginsAt[12] == 9);
    REQUIRE(map.rowBeginsAt[13] == 0);

    REQUIRE(map.colBeginsAt[0] == 0);
    REQUIRE(map.colBeginsAt[1] == 5);
    REQUIRE(map.colBeginsAt[2] == 5);
    REQUIRE(map.colBeginsAt[3] == 5);
    REQUIRE(map.colBeginsAt[4] == 5);
    REQUIRE(map.colBeginsAt[5] == 5);
    REQUIRE(map.colBeginsAt[6] == 5);
    REQUIRE(map.colBeginsAt[7] == 5);
    REQUIRE(map.colBeginsAt[8] == 5);
    REQUIRE(map.colBeginsAt[9] == 1);
    REQUIRE(map.colBeginsAt[10] == 1);
    REQUIRE(map.colBeginsAt[11] == 1);
    REQUIRE(map.colBeginsAt[12] == 1);
    REQUIRE(map.colBeginsAt[13] == 9);
    REQUIRE(map.colBeginsAt[14] == 9);
    REQUIRE(map.colBeginsAt[15] == 9);
    REQUIRE(map.colBeginsAt[16] == 9);
    REQUIRE(map.colBeginsAt[17] == 0);

    REQUIRE(map.rowEndsAt[0] == 0);
    REQUIRE(map.rowEndsAt[1] == 12);
    REQUIRE(map.rowEndsAt[2] == 12);
    REQUIRE(map.rowEndsAt[3] == 12);
    REQUIRE(map.rowEndsAt[4] == 12);
    REQUIRE(map.rowEndsAt[5] == 12);
    REQUIRE(map.rowEndsAt[6] == 12);
    REQUIRE(map.rowEndsAt[7] == 12);
    REQUIRE(map.rowEndsAt[8] == 12);
    REQUIRE(map.rowEndsAt[9] == 16);
    REQUIRE(map.rowEndsAt[10] == 16);
    REQUIRE(map.rowEndsAt[11] == 16);
    REQUIRE(map.rowEndsAt[12] == 16);
    REQUIRE(map.rowEndsAt[13] == 0);

    REQUIRE(map.colEndsAt[0] == 0);
    REQUIRE(map.colEndsAt[1] == 8);
    REQUIRE(map.colEndsAt[2] == 8);
    REQUIRE(map.colEndsAt[3] == 8);
    REQUIRE(map.colEndsAt[4] == 8);
    REQUIRE(map.colEndsAt[5] == 8);
    REQUIRE(map.colEndsAt[6] == 8);
    REQUIRE(map.colEndsAt[7] == 8);
    REQUIRE(map.colEndsAt[8] == 8);
    REQUIRE(map.colEndsAt[9] == 12);
    REQUIRE(map.colEndsAt[10] == 12);
    REQUIRE(map.colEndsAt[11] == 12);
    REQUIRE(map.colEndsAt[12] == 12);
    REQUIRE(map.colEndsAt[13] == 12);
    REQUIRE(map.colEndsAt[14] == 12);
    REQUIRE(map.colEndsAt[15] == 12);
    REQUIRE(map.colEndsAt[16] == 12);
    REQUIRE(map.colEndsAt[17] == 0);

    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, map.data[0]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '.', '.', '#', 0}, map.data[1]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '#', '.', '.', 0}, map.data[2]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '#', '.', '.', '.', 0}, map.data[3]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '.', '.', '.', 0}, map.data[4]));
    REQUIRE(areSame({0, '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '.', '#', 0}, map.data[5]));
    REQUIRE(areSame({0, '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', 0}, map.data[6]));
    REQUIRE(areSame({0, '.', '.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', 0}, map.data[7]));
    REQUIRE(areSame({0, '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', 0}, map.data[8]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '.', '.', '#', '.', '.', '.', '.', 0}, map.data[9]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '.', '.', '.', '.', '#', '.', '.', 0}, map.data[10]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '#', '.', '.', '.', '.', '.', '.', 0}, map.data[11]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, '.', '.', '.', '.', '.', '.', '#', '.', 0}, map.data[12]));
    REQUIRE(areSame({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, map.data[13]));

    REQUIRE(moves.size() == 13);

    REQUIRE(isMove<size_t>(moves[0], 10ull));
    REQUIRE(isMove<Direction>(moves[1], Direction::R));
    REQUIRE(isMove<size_t>(moves[2], 5ull));
    REQUIRE(isMove<Direction>(moves[3], Direction::L));
    REQUIRE(isMove<size_t>(moves[4], 5ull));
    REQUIRE(isMove<Direction>(moves[5], Direction::R));
    REQUIRE(isMove<size_t>(moves[6], 10ull));
    REQUIRE(isMove<Direction>(moves[7], Direction::L));
    REQUIRE(isMove<size_t>(moves[8], 4ull));
    REQUIRE(isMove<Direction>(moves[9], Direction::R));
    REQUIRE(isMove<size_t>(moves[10], 5ull));
    REQUIRE(isMove<Direction>(moves[11], Direction::L));
    REQUIRE(isMove<size_t>(moves[12], 5ull));
}

void solution()
{
    const auto [map, moves] = parse(std::cin);
    std::cout << solvePart1(map, moves) << std::endl;
    std::cout << solvePart2(map, moves, buildLargeBounds()) << std::endl;
}
}
