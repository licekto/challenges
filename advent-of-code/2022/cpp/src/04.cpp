#include "04.hpp"

#include <catch2/catch_all.hpp>

#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

namespace aoc04
{
namespace
{
using T = int64_t;

using Range = std::pair<T, T>;

bool IsFullyContainedIntreval(const Range& r1, const Range& r2)
{
    return r1.first <= r2.first && r1.second >= r2.second;
}

bool OverlappingInterval(const Range& r1, const Range& r2)
{
    return (r1.first <= r2.first && r1.second >= r2.first) || (r1.first <= r2.second && r1.second >= r2.first);
}

struct Assignment
{
    Assignment();
    Assignment(const T rl1, const T rr1, const T rl2, const T rr2);
    Range r1, r2;
};

Assignment::Assignment()
    : r1{0, 0}
    , r2{0, 0}
{}

Assignment::Assignment(const T rl1, const T rr1, const T rl2, const T rr2)
    : r1{rl1, rr1}
    , r2{rl2, rr2}
{}

bool IsFullyContained(const Assignment& assignment)
{
    return IsFullyContainedIntreval(assignment.r1, assignment.r2)
        || IsFullyContainedIntreval(assignment.r2, assignment.r1);
}

bool Overlaps(const Assignment& assignment)
{
    return OverlappingInterval(assignment.r1, assignment.r2)
        || OverlappingInterval(assignment.r2, assignment.r1);
}

std::istream& operator>>(std::istream& is, Range& range)
{
    static constexpr char separator = '-';
    char c;

    is >> range.first;
    if (is.eof())
    {
        return is;
    }
    is >> c;
    if (c != separator)
    {
        throw std::runtime_error("Wrong separator: '" + std::string(c, 1) + "'");
    }
    is >> range.second;

    if (range.first > range.second)
    {
        throw std::runtime_error("The lower bound of the interval can't be greater than the upper bound, interval: ["
                                 + std::to_string(range.first) + "," + std::to_string(range.first) + "]");
    }

    return is;
}

std::istream& operator>>(std::istream& is, Assignment& assignment)
{
    static constexpr char separator = ',';
    char c;

    is >> assignment.r1;
    if (is.eof())
    {
        return is;
    }
    is >> c;
    if (c != separator)
    {
        throw std::runtime_error("Wrong separator: '" + std::string(c, 1) + "'");
    }
    is >> assignment.r2;

    return is;
}

using Data = std::vector<Assignment>;

Data parse(std::istream& is)
{
    Data data;

    Assignment a;
    while (1)
    {
        is >> a;
        if (is.eof())
        {
            break;
        }
        data.push_back(a);
    }

    return data;
}

T solve(const Data& data, std::function<bool(const Assignment&)> condition)
{
    T count = 0;

    for (const auto& assignment : data)
    {
        if (condition(assignment))
        {
            ++count;
        }
    }

    return count;
}

T solvePart1(const Data& data)
{
    return solve(data, IsFullyContained);
}

T solvePart2(const Data& data)
{
    return solve(data, Overlaps);
}

void testAssignment(const Assignment& assignment, const std::vector<T>& values)
{
    REQUIRE(assignment.r1.first  == values[0]);
    REQUIRE(assignment.r1.second == values[1]);
    REQUIRE(assignment.r2.first  == values[2]);
    REQUIRE(assignment.r2.second == values[3]);
}

}

TEST_CASE("2022-day-04")
{
    std::stringstream ss;
    ss << R"(2-4,6-8
             2-3,4-5
             5-7,7-9
             2-8,3-7
             6-6,4-6
             2-6,4-8)" << std::endl;

    const auto data = parse(ss);

    REQUIRE(data.size() == 6);
    testAssignment(data[0], {2, 4, 6, 8});
    testAssignment(data[1], {2, 3, 4, 5});
    testAssignment(data[2], {5, 7, 7, 9});
    testAssignment(data[3], {2, 8, 3, 7});
    testAssignment(data[4], {6, 6, 4, 6});
    testAssignment(data[5], {2, 6, 4, 8});

    REQUIRE(IsFullyContained(Assignment{}));
    REQUIRE(IsFullyContained(Assignment(1, 1, 1, 1)));
    REQUIRE(IsFullyContained(Assignment(6, 6, 4, 7)));
    REQUIRE(IsFullyContained(Assignment(4, 8, 1, 9)));
    REQUIRE(IsFullyContained(Assignment(4, 8, 1, 9)));

    REQUIRE(!IsFullyContained(Assignment(1, 2, 2, 3)));
    REQUIRE(!IsFullyContained(Assignment(1, 2, 3, 4)));
    REQUIRE(!IsFullyContained(Assignment(1, 2, 5, 6)));
    REQUIRE(!IsFullyContained(Assignment(1, 5, 3, 8)));

    REQUIRE(!IsFullyContained(Assignment(7, 8, 6, 7)));
    REQUIRE(!IsFullyContained(Assignment(7, 8, 5, 6)));
    REQUIRE(!IsFullyContained(Assignment(7, 8, 4, 5)));
    REQUIRE(!IsFullyContained(Assignment(3, 8, 1, 5)));

    REQUIRE(Overlaps(Assignment(1, 1, 1, 1)));
    REQUIRE(Overlaps(Assignment(1, 2, 2, 3)));
    REQUIRE(Overlaps(Assignment(1, 5, 4, 8)));

    REQUIRE(!Overlaps(Assignment(1, 5, 6, 8)));
    REQUIRE(!Overlaps(Assignment(6, 8, 1, 4)));

    REQUIRE(solvePart1(data) == 2);
    REQUIRE(solvePart2(data) == 4);
}

void solution()
{
    const auto data = parse(std::cin);
    std::cout << solvePart1(data) << std::endl;
    std::cout << solvePart2(data) << std::endl;
}
}
