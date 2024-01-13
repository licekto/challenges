#include "23.hpp"

#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <sstream>
#include <vector>

namespace aoc23
{
namespace
{
using Id = size_t;
constexpr size_t DirectionSize = 8;
enum class Direction : int64_t { N = 0, NE = 1, E = 2, SE = 3, S = 4, SW = 5, W = 6, NW = 7 };

Direction toDir(const size_t val)
{
    if (val >= DirectionSize)
    {
        throw std::runtime_error("Unknown direction: '" + std::to_string(val) + "'");
    }
    return static_cast<Direction>(val);
}

struct DirCycle
{
    Direction get() const;
    DirCycle& operator++();
private:
    static constexpr size_t size = 4;
    static constexpr std::array<Direction, size> dirs = { Direction::N, Direction::S, Direction::W, Direction::E };
    size_t i = 0;
};

Direction DirCycle::get() const
{
    return dirs[i];
}

DirCycle& DirCycle::operator++()
{
    i = (i + 1) % size;
    return *this;
}

Direction operator+(const Direction lhs, const int64_t val)
{
    return static_cast<Direction>((static_cast<uint8_t>(lhs) + val) % DirectionSize);
}

Direction operator-(const Direction lhs, const int64_t val)
{
    int64_t tmp = static_cast<uint8_t>(lhs) - val;
    if (tmp < 0)
    {
        tmp = DirectionSize + tmp;
    }
    return static_cast<Direction>(tmp);
}

Utility::Coord operator+(Utility::Coord c, const Direction dir)
{
    switch(dir)
    {
    case Direction::N  : --c.y; break;
    case Direction::NE : --c.y; ++c.x; break;
    case Direction::E  : ++c.x; break;
    case Direction::SE : ++c.y; ++c.x; break;
    case Direction::S  : ++c.y; break;
    case Direction::SW : ++c.y; --c.x; break;
    case Direction::W  : --c.x; break;
    case Direction::NW : --c.y; --c.x; break;
    }
    return c;
}

class Elves
{
public:
    using Proposals = std::unordered_map<Id, Utility::Coord>;
    struct BoundingBox
    {
        Utility::Coord topLeft, bottomRight;
    };

    Elves(Utility::CoordMap<Id> coordToId, std::unordered_map<Id, Utility::Coord> idToCoord);
    bool isSurrounded(const Utility::Coord& c) const;
    bool directionContains(const Utility::Coord& c, const Direction dir) const;
    Proposals propose() const;
    bool exec(const Elves::Proposals& proposals);
    void incDir();
    BoundingBox getBoundingBox() const;
    size_t calculateFreeInBoundingBox() const;
    void printBoundingBox(std::ostream& os) const;
    const Utility::CoordMap<Id>& getCoordToId() const;

private:
    Utility::CoordMap<Id> coordToId, prev;
    std::unordered_map<Id, Utility::Coord> idToCoord;
    DirCycle dirCycle {};
};

Elves::Elves(Utility::CoordMap<Id> coordToId, std::unordered_map<Id, Utility::Coord> idToCoord)
    : coordToId(std::move(coordToId))
    , idToCoord(std::move(idToCoord))
{

}

bool Elves::isSurrounded(const Utility::Coord& c) const
{
    for (size_t i = 0; i < DirectionSize; ++i)
    {
        if (const auto it = coordToId.find(c + toDir(i)); it != std::end(coordToId))
        {
            return true;
        }
    }
    return false;
}

bool Elves::directionContains(const Utility::Coord& c, const Direction dir) const
{
    for (const auto d : {dir - 1, dir, dir + 1})
    {
        if (const auto it = coordToId.find(c + d); it != std::end(coordToId))
        {
            return true;
        }
    }
    return false;
}

Elves::Proposals Elves::propose() const
{
    Utility::CoordMap<std::vector<Id>> proposedCoords;
    Proposals proposals;

    for (const auto& [coord, id] : coordToId)
    {
        if (!isSurrounded(coord))
        {
            continue;
        }

        auto currentDirCycle = dirCycle;

        for (size_t i = 0; i < 4; ++i)
        {
            const auto proposedDir = currentDirCycle.get();
            const auto proposedCoord = coord + proposedDir;
            if (!directionContains(coord, proposedDir))
            {
                proposals[id] = proposedCoord;
                proposedCoords[proposedCoord].push_back(id);
                break;
            }
            ++currentDirCycle;
        }
    }

    for (const auto& [c, ids] : proposedCoords)
    {
        if (ids.size() > 1)
        {
            for (const auto id : ids)
            {
                proposals.erase(id);
            }
        }
    }

    return proposals;
}

bool Elves::exec(const Elves::Proposals& proposals)
{
    prev = coordToId;
    for (const auto& [id, newCoord] : proposals)
    {
        const auto& oldCoord = idToCoord[id];
        coordToId.erase(oldCoord);
        coordToId[newCoord] = id;
        idToCoord[id] = newCoord;
    }

    return prev != coordToId;
}


void Elves::incDir()
{
    ++dirCycle;
}

Elves::BoundingBox Elves::getBoundingBox() const
{
    Utility::T minx = Utility::Coord::Max, miny = Utility::Coord::Max;
    Utility::T maxx = Utility::Coord::Min, maxy = Utility::Coord::Min;

    for (const auto& [c, id] : coordToId)
    {
        minx = std::min(minx, c.x);
        maxx = std::max(maxx, c.x);
        miny = std::min(miny, c.y);
        maxy = std::max(maxy, c.y);
    }

    return {{minx - 1, miny - 1}, {maxx, maxy}};
}

size_t Elves::calculateFreeInBoundingBox() const
{
    const auto boundingBox = getBoundingBox();
    const size_t totalSize = (boundingBox.bottomRight.x - boundingBox.topLeft.x) * (boundingBox.bottomRight.y - boundingBox.topLeft.y);
    return totalSize - idToCoord.size();
}

void Elves::printBoundingBox(std::ostream& os) const
{
    const auto boundingBox = getBoundingBox();
    const int64_t margin = 0;

    os << "  ";
    for (int64_t i = boundingBox.topLeft.x; i <= boundingBox.bottomRight.x + margin; ++i)
    {
        os << (i >= 10 ? std::to_string(i / 10) : " ");
    }
    os << std::endl << "  ";

    for (int64_t i = boundingBox.topLeft.x; i <= boundingBox.bottomRight.x + margin; ++i)
    {
        os << i % 10;
    }
    os << std::endl;

    for (int64_t i = boundingBox.topLeft.x; i <= boundingBox.bottomRight.y + margin; ++i)
    {
        if (i < 10)
        {
            os << " ";
        }
        os << i;
        for (int64_t j = boundingBox.topLeft.y; j <= boundingBox.bottomRight.x + margin; ++j)
        {
            os << (coordToId.contains({j, i}) ? "#" : ".");
        }
        os << std::endl;
    }
}

const Utility::CoordMap<Id>& Elves::getCoordToId() const
{
    return coordToId;
}

Elves parse(std::istream& is)
{
    Utility::CoordMap<size_t> coordToId;
    std::unordered_map<Id, Utility::Coord> idToCoord;

    std::string line;
    int64_t y = 0;
    size_t id = 1;

    while (std::getline(is, line))
    {
        for (size_t x = 0; x < line.size(); ++x)
        {
            if (line[x] == '#')
            {
                const Utility::Coord c{static_cast<Utility::T>(x), y};
                coordToId[c] = id;
                idToCoord[id] = c;
                ++id;
            }
        }
        ++y;
    }

    return Elves(coordToId, idToCoord);
}

std::pair<size_t, size_t> solve(Elves elves)
{
    size_t i = 1;
    for (; i <= 10; ++i)
    {
        elves.exec(elves.propose());
        elves.incDir();
    }

    const size_t freeAfter10 = elves.calculateFreeInBoundingBox();

    while (elves.exec(elves.propose()))
    {
        elves.incDir();
        ++i;
    }

    return {freeAfter10, i};
}
}

/*
            1111
  01234567890123
 0..............
 1..............
 2.......#......
 3.....###.#....
 4...#...#.#....
 5....#...##....
 6...#.###......
 7...##.#.##....
 8....#..#......
 9..............
10..............
11..............
*/

TEST_CASE("2022-day-23-part1")
{
    auto input = Utility::getTestData("23/test-input.txt");
    auto elves = parse(input);

    const auto [freeAfter10, stepsToNoMove] = solve(elves);

    REQUIRE(freeAfter10 == 110);
    REQUIRE(stepsToNoMove == 20);
}

TEST_CASE("2022-day-23-util")
{
    auto input = Utility::getTestData("23/test-input.txt");
    auto elves = parse(input);

    {
        const auto &data = elves.getCoordToId();
        REQUIRE(data.size() == 22);
        REQUIRE(data.at({7, 2}) == 1);
        REQUIRE(data.at({5, 3}) == 2);
        REQUIRE(data.at({6, 3}) == 3);
        REQUIRE(data.at({7, 3}) == 4);
        REQUIRE(data.at({9, 3}) == 5);
        REQUIRE(data.at({3, 4}) == 6);
        REQUIRE(data.at({7, 4}) == 7);
        REQUIRE(data.at({9, 4}) == 8);
        REQUIRE(data.at({4, 5}) == 9);
        REQUIRE(data.at({8, 5}) == 10);
        REQUIRE(data.at({9, 5}) == 11);
        REQUIRE(data.at({3, 6}) == 12);
        REQUIRE(data.at({5, 6}) == 13);
        REQUIRE(data.at({6, 6}) == 14);
        REQUIRE(data.at({7, 6}) == 15);
        REQUIRE(data.at({3, 7}) == 16);
        REQUIRE(data.at({4, 7}) == 17);
        REQUIRE(data.at({6, 7}) == 18);
        REQUIRE(data.at({8, 7}) == 19);
        REQUIRE(data.at({9, 7}) == 20);
        REQUIRE(data.at({4, 8}) == 21);
        REQUIRE(data.at({7, 8}) == 22);
    }

    Utility::Coord c{1, 1};

    REQUIRE(c + Direction::N  == Utility::Coord{1, 0});
    REQUIRE(c + Direction::NE == Utility::Coord{2, 0});
    REQUIRE(c + Direction::E  == Utility::Coord{2, 1});
    REQUIRE(c + Direction::SE == Utility::Coord{2, 2});
    REQUIRE(c + Direction::S  == Utility::Coord{1, 2});
    REQUIRE(c + Direction::SW == Utility::Coord{0, 2});
    REQUIRE(c + Direction::W  == Utility::Coord{0, 1});
    REQUIRE(c + Direction::NW == Utility::Coord{0, 0});

    REQUIRE(Direction::N - 1 == Direction::NW);
    REQUIRE(Direction::NW + 1 == Direction::N);
    REQUIRE(Direction::N + 8 == Direction::N);
    REQUIRE(Direction::N - 8 == Direction::N);

    const auto proposals = elves.propose();

    REQUIRE(proposals.size() == 11);
    REQUIRE(proposals.at(1) == Utility::Coord{7, 1});

    elves.exec(proposals);
}

void solution()
{
    auto elves = parse(std::cin);

    const auto [freeAfter10, stepsToNoMove] = solve(elves);

    std::cout << freeAfter10 << std::endl;
    std::cout << stepsToNoMove << std::endl;
}
}
