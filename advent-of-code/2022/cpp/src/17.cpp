#include "17.hpp"

#include <catch2/catch_all.hpp>

#include <bitset>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "Utility.hpp"

namespace aoc17
{
namespace
{

constexpr size_t width = 7;

using Points = std::vector<Utility::Coord>;

struct Shape
{
    Utility::Coord bottomLeft {};
    Points points {};
    size_t w = 0, h = 0;

    bool moveOneLeft();
    bool moveOneRight();
    bool moveOneUp();
    bool moveOneDown();

    bool moveSideways(const char dir);
};

bool Shape::moveOneLeft()
{
    if (bottomLeft.x <= 0)
    {
        return false;
    }
    for (auto& point : points)
    {
        --point.x;
    }
    --bottomLeft.x;
    return true;
}

bool Shape::moveOneRight()
{
    if (bottomLeft.x + w >= width)
    {
        return false;
    }
    for (auto& point : points)
    {
        ++point.x;
    }
    ++bottomLeft.x;
    return true;
}

bool Shape::moveOneUp()
{
    for (auto& point : points)
    {
        ++point.y;
    }
    ++bottomLeft.y;
    return true;
}

bool Shape::moveOneDown()
{
    if (bottomLeft.y <= 0)
    {
        return false;
    }
    for (auto& point : points)
    {
        --point.y;
    }
    --bottomLeft.y;
    return true;
}

bool Shape::moveSideways(const char dir)
{
    if (dir == '>')
    {
        return moveOneRight();
    }
    else
    {
        return moveOneLeft();
    }
}

Shape getShape(const Utility::Coord init, Points points, const size_t w, const size_t h)
{
    for (auto& coord : points)
    {
        coord.x += init.x;
        coord.y += init.y;
    }

    Shape s
    {
        .bottomLeft = init,
        .points = points,
        .w = w,
        .h = h
    };

    return s;
}

Shape getShape1(const Utility::Coord init)
{
    return getShape(init, { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, 4, 1);
}

Shape getShape2(const Utility::Coord init)
{
    return getShape(init, { {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1} }, 3, 3);
}

Shape getShape3(const Utility::Coord init)
{
    return getShape(init, { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2} }, 3, 3);
}

Shape getShape4(const Utility::Coord init)
{
    return getShape(init, { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, 1, 4);
}

Shape getShape5(const Utility::Coord init)
{
    return getShape(init, { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, 2, 2);
}

std::ostream& operator<<(std::ostream& os, const Shape& shape)
{
    std::vector<std::vector<Utility::Coord>> points;
    points.resize(shape.h);
    for (const auto& point : shape.points)
    {
        points[point.y - shape.bottomLeft.y].push_back(point);
    }

    bool first = true;
    for (auto it = std::rbegin(points); it != std::rend(points); ++it)
    {
        if (!first)
        {
            os << std::endl;
        }
        first = false;

        std::string rowStr = ".......";
        for (const auto& point : *it)
        {
            rowStr[point.x] = '#';
        }
        os << "|" << rowStr << "|";
    }

    return os;
}

struct Chamber
{
    std::unordered_set<Utility::Coord, Utility::CoordHash> points;
    size_t height = 0;

    bool isCollision(const Shape& shape) const;
    void add(const Shape& shape);
};

bool Chamber::isCollision(const Shape& shape) const
{
    for (const auto& point : shape.points)
    {
        if (points.count(point))
        {
            return true;
        }
    }
    return false;
}

void Chamber::add(const Shape& shape)
{
    for (const auto& point : shape.points)
    {
        points.insert(point);
        height = std::max(static_cast<size_t>(point.y) + 1, height);
    }
}

std::ostream& operator<<(std::ostream& os, const Chamber& chamber)
{
    std::vector<std::vector<Utility::Coord>> points;
    points.resize(chamber.height + 1);

    for (const auto& point : chamber.points)
    {
        points[point.y].push_back(point);
    }

    for (auto it = std::rbegin(points); it != std::rend(points); ++it)
    {
        std::string rowStr = ".......";
        for (const auto& point : *it)
        {
            rowStr[point.x] = '#';
        }
        os << "|" << rowStr << "|" << std::endl;
    }

    os << "+-------+" << std::endl;

    return os;
}

char getOpposite(const char dir)
{
    return dir == '<' ? '>' : '<';
}

class Executor
{
public:
    struct State
    {
        const size_t jetIndex, rockIndex, rockDelta, height, leftX;
    };

    struct StateHash
    {
        uint64_t operator()(const State& state) const
        {
            std::string str;

            str.append(std::to_string(state.jetIndex))
               .append(std::to_string(state.rockIndex))
               .append(std::to_string(state.rockDelta));

            return std::hash<std::string>{}(str);
        }
    };

    Executor(const std::string& jets);
    size_t getHeight() const;
    State putRock();

private:
    const std::string jets;
    Chamber chamber;
    std::vector<std::function<Shape(const Utility::Coord)>> shapeGen;
    const size_t jetsCount, shapesCount;
    size_t jetIdx = 0, shapeGenIdx = 0;
};

bool operator==(const Executor::State& s1, const Executor::State& s2)
{
    return s1.jetIndex == s2.jetIndex && s1.rockIndex == s2.rockIndex && s1.rockDelta == s2.rockDelta;
}

bool operator!=(const Executor::State& s1, const Executor::State& s2)
{
    return !(s1 == s2);
}

std::ostream& operator<<(std::ostream& os, const Executor::State& s)
{
    return os << "ji=" << s.jetIndex << ",ri=" << s.rockIndex << ",rd=" << s.rockDelta << ",h=" << s.height << ",x=" << s.leftX;
}

Executor::Executor(const std::string& jets)
    : jets(jets)
    , shapeGen{ getShape1, getShape2, getShape3, getShape4, getShape5 }
    , jetsCount(jets.size())
    , shapesCount(shapeGen.size())
{}

size_t Executor::getHeight() const
{
    return chamber.height;
}

Executor::State Executor::putRock()
{
    const Utility::Coord init = {2, static_cast<Utility::T>(chamber.height) + 3};
    auto rock = shapeGen[shapeGenIdx](init);

    //std::cout << "shape no. " << shapeGenIdx + 1 << std::endl;
    //std::cout << rock << std::endl << "moving..." << std::endl;

    const size_t rockInitHeight = rock.bottomLeft.y;

    while (1)
    {
        const auto direction = jets[jetIdx];

        const auto movedSideways = rock.moveSideways(direction);
        if (movedSideways && chamber.isCollision(rock))
        {
            rock.moveSideways(getOpposite(direction));
        }

        //std::cout << direction << std::endl;
        //std::cout << rock << std::endl;

        ++jetIdx;
        jetIdx = jetIdx % jetsCount;

        const auto movedDown = rock.moveOneDown();
        if (!movedDown)
        {
            chamber.add(rock);
            break;
        }
        if (chamber.isCollision(rock))
        {
            rock.moveOneUp();
            chamber.add(rock);
            break;
        }
    }

    //std::cout << std::endl << "chamber:" << std::endl << chamber << std::endl;
    //std::cout << "===================" << std::endl << std::endl;

    const auto stateShape = shapeGenIdx;

    ++shapeGenIdx;
    shapeGenIdx = shapeGenIdx % shapesCount;

    return {jetIdx, stateShape, rockInitHeight - rock.bottomLeft.y, chamber.height, static_cast<size_t>(rock.bottomLeft.x)};
}

Utility::T solvePart1(const std::string& jets)
{
    Executor executor(jets);

    for (size_t i = 0; i < 2022; ++i)
    {
        executor.putRock();
    }

    return executor.getHeight();
}

size_t solvePart2(const std::string& jets)
{
    static constexpr size_t historyCheck = 5;
    static constexpr size_t steps = 1000000000000;

    Executor executor(jets);
    std::vector<Executor::State> states;
    std::unordered_map<Executor::State, size_t, Executor::StateHash> history;
    size_t period = 0, periodHeight = 0;

    size_t stepNo = 0;
    for (; stepNo < steps; ++stepNo)
    {
        const auto state = executor.putRock();
        //std::cout << "s=" << stepNo << ":" << state;
        if (const auto it = history.find(state); it != std::end(history))
        {
            bool matches = true;
            for (size_t i = 0; i < historyCheck; ++i)
            {
                if (states[stepNo - i - 1] != states[it->second - i - 1])
                {
                    matches = false;
                    break;
                }
            }

            if (matches)
            {
                period = stepNo - it->second;
                periodHeight = executor.getHeight() - it->first.height;
                //std::cout << " F" << std::endl;
                break;
            }
        }
        //std::cout << std::endl;
        history[state] = stepNo;
        states.push_back(state);
    }
    ++stepNo;

    //std::cout << "p=" << period << ",ph=" << periodHeight << ",r=" << steps % period << ",exH=" << executor.getHeight() << std::endl;

    const auto skipped = (steps - stepNo) / period;
    const auto remaining = skipped * period;
    stepNo += remaining;

    for (; stepNo < steps; ++stepNo)
    {
        executor.putRock();
    }

    const auto result = executor.getHeight() + (skipped * periodHeight);

    //std::cout << executor.getHeight() << " + (" << skipped << " * " << periodHeight << ") = " << result << std::endl;

    return result;
}

std::string parse(std::istream& is)
{
    std::string line;
    std::getline(is, line);
    return line;
}

}

TEST_CASE("2022-day-17")
{
    std::stringstream ss;
    ss << ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
    const auto jets = parse(ss);
    REQUIRE(solvePart1(jets) == 3068);
    REQUIRE(solvePart2(jets) == 1514285714288);
}

void solution()
{
    const auto jets = parse(std::cin);
    std::cout << solvePart1(jets) << std::endl;
    std::cout << solvePart2(jets) << std::endl;
}
}
