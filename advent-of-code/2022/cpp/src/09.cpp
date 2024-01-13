#include "09.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace aoc09
{
namespace
{
struct Move
{
    enum class Direction { R, L, U, D };

    static Direction charToDir(const char c);

    Direction direction;
    size_t count;
};

Move::Direction Move::charToDir(const char c)
{
    switch (c)
    {
    case 'R' : return Direction::R;
    case 'L' : return Direction::L;
    case 'U' : return Direction::U;
    case 'D' : return Direction::D;
    }
    throw std::runtime_error("Unknown direction: '" + std::string(c, 1) + "'");
}

std::istream& operator>>(std::istream& is, Move& move)
{
    char c;
    is >> c;
    if (is.eof())
    {
        return is;
    }
    is >> move.count;
    move.direction = Move::charToDir(c);

    return is;
}

using Moves = std::vector<Move>;

Moves parse(std::istream& is)
{
    Moves moves;

    Move move;
    while (1)
    {
        is >> move;
        if (is.eof())
        {
            break;
        }
        moves.push_back(move);
    }

    return moves;
}

void DoMove(Utility::Coord& coord, const Move::Direction& direction)
{
    switch (direction)
    {
    case Move::Direction::U : ++coord.y; break;
    case Move::Direction::D : --coord.y; break;
    case Move::Direction::R : ++coord.x; break;
    case Move::Direction::L : --coord.x; break;
    }
}

size_t distance(const Utility::Coord& p1, const Utility::Coord& p2)
{
    return std::max(std::abs(p1.x - p2.x), std::abs(p1.y - p2.y));
}

uint64_t hash(const Utility::Coord& point)
{
    uint64_t result = *reinterpret_cast<const uint32_t *>(&point.x);
    result <<= 32;
    return result | *reinterpret_cast<const uint32_t *>(&point.y);
}

Moves getDirection(const Utility::Coord& head, const Utility::Coord& tail)
{
    Moves moves;

    if (head.y > tail.y)
    {
        moves.push_back({Move::Direction::U, 1});
    }
    else if (head.y < tail.y)
    {
        moves.push_back({Move::Direction::D, 1});
    }

    if (head.x > tail.x)
    {
        moves.push_back({Move::Direction::R, 1});
    }
    else if (head.x < tail.x)
    {
        moves.push_back({Move::Direction::L, 1});
    }

    return moves;
}

class VectorSet
{
public:
    void Set(const Utility::Coord& point)
    {
        set.push_back({point.x, point.y});
    }

    size_t Size()
    {
        std::sort(std::begin(set), std::end(set));
        const auto last = std::unique(std::begin(set), std::end(set));
        return last - std::begin(set);
    }

private:
    std::vector<std::pair<Utility::T, Utility::T>> set;
};

class UnorderedSet
{
public:
    void Set(const Utility::Coord& point)
    {
        set.insert(hash(point));
    }

    size_t Size()
    {
        return set.size();
    }

private:
    std::unordered_set<uint64_t> set;
};

template <typename SetT = UnorderedSet>
size_t solve(const Moves& headMoves, const size_t pointsCount)
{
    SetT set;
    std::vector<Utility::Coord> points(pointsCount, Utility::Coord{});

    for (const auto& headMove : headMoves)
    {
        for (size_t i = 0; i < headMove.count; ++i)
        {
            DoMove(points[0], headMove.direction);

            for (auto it = std::begin(points) + 1; it != std::end(points); ++it)
            {
                if (distance(*it, *(it - 1)) == 2)
                {
                    const auto tailMoves = getDirection(*(it - 1), *it);
                    for (const auto tailMove : tailMoves)
                    {
                        DoMove(*it, tailMove.direction);
                    }
                }
            }

            set.Set(points.back());
        }
    }

    return set.Size();
}

size_t solvePart1(const Moves& headMoves)
{
    return solve(headMoves, 2);
}

size_t solvePart2(const Moves& headMoves)
{
    return solve(headMoves, 10);
}

}

TEST_CASE("2022-day-09")
{
    std::stringstream ss;
    ss << "R 4" << std::endl
       << "U 4" << std::endl
       << "L 3" << std::endl
       << "D 1" << std::endl
       << "R 4" << std::endl
       << "D 1" << std::endl
       << "L 5" << std::endl
       << "R 2" << std::endl;

    const auto moves = parse(ss);

    REQUIRE(moves.size() == 8);
    REQUIRE(moves[0].direction == Move::Direction::R);
    REQUIRE(moves[0].count == 4);
    REQUIRE(moves[1].direction == Move::Direction::U);
    REQUIRE(moves[1].count == 4);
    REQUIRE(moves[2].direction == Move::Direction::L);
    REQUIRE(moves[2].count == 3);
    REQUIRE(moves[3].direction == Move::Direction::D);
    REQUIRE(moves[3].count == 1);
    REQUIRE(moves[4].direction == Move::Direction::R);
    REQUIRE(moves[4].count == 4);
    REQUIRE(moves[5].direction == Move::Direction::D);
    REQUIRE(moves[5].count == 1);
    REQUIRE(moves[6].direction == Move::Direction::L);
    REQUIRE(moves[6].count == 5);
    REQUIRE(moves[7].direction == Move::Direction::R);
    REQUIRE(moves[7].count == 2);

    REQUIRE(hash(Utility::Coord{1, 1}) == 0b100000000000000000000000000000001);
    REQUIRE(hash(Utility::Coord{2, 1}) == 0b1000000000000000000000000000000001);
    REQUIRE(hash(Utility::Coord{2, 2}) == 0b1000000000000000000000000000000010);

    Utility::Coord p{0, 0};
    DoMove(p, Move::Direction::D);
    REQUIRE(p.x == 0);
    REQUIRE(p.y == -1);
    DoMove(p, Move::Direction::L);
    REQUIRE(p.x == -1);
    REQUIRE(p.y == -1);
    DoMove(p, Move::Direction::U);
    REQUIRE(p.x == -1);
    REQUIRE(p.y == 0);
    DoMove(p, Move::Direction::R);
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 0);

    REQUIRE(distance({}, {}) == 0);
    REQUIRE(distance({0, 0}, {1, 0}) == 1);
    REQUIRE(distance({0, 0}, {0, 1}) == 1);
    REQUIRE(distance({0, 0}, {-1, 0}) == 1);
    REQUIRE(distance({0, 0}, {0, -1}) == 1);
    REQUIRE(distance({0, 0}, {1, 1}) == 1);
    REQUIRE(distance({0, 0}, {-1, -1}) == 1);
    REQUIRE(distance({0, 0}, {1, -1}) == 1);
    REQUIRE(distance({0, 0}, {-1, 1}) == 1);

    REQUIRE(distance({0, 0}, {0, 2}) == 2);
    REQUIRE(distance({0, 0}, {1, 2}) == 2);
    REQUIRE(distance({0, 0}, {2, 2}) == 2);
    REQUIRE(distance({0, 0}, {2, 1}) == 2);
    REQUIRE(distance({0, 0}, {2, 0}) == 2);
    REQUIRE(distance({0, 0}, {2, -1}) == 2);
    REQUIRE(distance({0, 0}, {2, -2}) == 2);
    REQUIRE(distance({0, 0}, {1, -2}) == 2);
    REQUIRE(distance({0, 0}, {0, -2}) == 2);
    REQUIRE(distance({0, 0}, {-1, -2}) == 2);
    REQUIRE(distance({0, 0}, {-2, -2}) == 2);
    REQUIRE(distance({0, 0}, {-2, -2}) == 2);
    REQUIRE(distance({0, 0}, {-2, -1}) == 2);
    REQUIRE(distance({0, 0}, {-2, 0}) == 2);
    REQUIRE(distance({0, 0}, {-2, 1}) == 2);
    REQUIRE(distance({0, 0}, {-2, 2}) == 2);
    REQUIRE(distance({0, 0}, {-1, 2}) == 2);

    Utility::Coord h{0, 0}, t{0, 0};
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 0);
    }

    h.x = 2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].direction == Move::Direction::R);
    }

    h.x = -2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].direction == Move::Direction::L);
    }

    h.x = 0;
    h.y = 2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].direction == Move::Direction::U);
    }

    h.y = -2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].direction == Move::Direction::D);
    }

    h.x = 1;
    h.y = 2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::U);
        REQUIRE(moves[1].direction == Move::Direction::R);
    }

    h.x = -1;
    h.y = 2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::U);
        REQUIRE(moves[1].direction == Move::Direction::L);
    }

    h.x = 1;
    h.y = -2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::D);
        REQUIRE(moves[1].direction == Move::Direction::R);
    }

    h.x = -1;
    h.y = -2;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::D);
        REQUIRE(moves[1].direction == Move::Direction::L);
    }

    h.x = 2;
    h.y = 1;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::U);
        REQUIRE(moves[1].direction == Move::Direction::R);
    }

    h.x = 2;
    h.y = -1;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::D);
        REQUIRE(moves[1].direction == Move::Direction::R);
    }

    h.x = -2;
    h.y = 1;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::U);
        REQUIRE(moves[1].direction == Move::Direction::L);
    }

    h.x = -2;
    h.y = -1;
    {
        const auto moves = getDirection(h, t);
        REQUIRE(moves.size() == 2);
        REQUIRE(moves[0].direction == Move::Direction::D);
        REQUIRE(moves[1].direction == Move::Direction::L);
    }

    REQUIRE(solvePart1(moves) == 13);
    REQUIRE(solvePart2(moves) == 1);

    ss.clear();
    ss << "R 5" << std::endl
       << "U 8" << std::endl
       << "L 8" << std::endl
       << "D 3" << std::endl
       << "R 17" << std::endl
       << "D 10" << std::endl
       << "L 25" << std::endl
       << "U 20" << std::endl;

    const auto moves2 = parse(ss);
    REQUIRE(solvePart2(moves2) == 36);
}

template <typename SetT>
void measureType(const std::string& name, const size_t iterations, const Moves& moves)
{
    const auto times = Utility::measure<std::chrono::microseconds>(iterations, [&]()
    {
        solve<SetT>(moves, 10);
    });

    const auto stats = Utility::calculateStats(times);
    std::cout << name << ": iterations=" << iterations << ", mean=" << stats.mean << "us, standardDeviation=" << stats.standardDeviation << "us" << std::endl;
}

void benchmark(const Moves& moves)
{
    static constexpr size_t iterations = 100;

    measureType<VectorSet>("vector", iterations, moves);
    measureType<UnorderedSet>("unordered_set", iterations, moves);
}

void solution()
{
    const auto moves = parse(std::cin);
    std::cout << solvePart1(moves) << std::endl;
    std::cout << solvePart2(moves) << std::endl;

    //benchmark(moves);
}
}
