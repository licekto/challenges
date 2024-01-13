#include "08.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <sstream>
#include <vector>

namespace aoc08
{
namespace
{
using T = uint32_t;
using Row = std::vector<T>;
using Grid = std::vector<Row>;

Grid parse(std::istream& is)
{
    Grid grid;

    std::string line;

    while (std::getline(is, line))
    {
        Row row;

        for (const auto c : line)
        {
            row.push_back(c - '0');
        }

        grid.push_back(row);
    }

    return grid;
}

size_t solveRow(const Grid& grid, Grid& map, const size_t r)
{
    size_t sum = 0;

    T max = grid[r][0];
    for (size_t i = 1; i < grid[r].size() - 1; ++i)
    {
        if (grid[r][i] > max)
        {
            max = grid[r][i];
            if (map[r][i] == 0)
            {
                ++sum;
                map[r][i] = 1;
            }
        }
    }

    max = grid[r].back();
    for (size_t i = grid[r].size() - 2; i > 0; --i)
    {
        if (grid[r][i] > max)
        {
            max = grid[r][i];
            if (map[r][i] == 0)
            {
                ++sum;
                map[r][i] = 1;
            }
        }
    }

    return sum;
}

size_t solveCol(const Grid& grid, Grid& map, const size_t c)
{
    size_t sum = 0;

    T max = grid[0][c];
    for (size_t i = 1; i < grid.size() - 1; ++i)
    {
        if (grid[i][c] > max)
        {
            max = grid[i][c];
            if (map[i][c] == 0)
            {
                ++sum;
                map[i][c] = 1;
            }
        }
    }

    max = grid.back()[c];
    for (size_t i = grid.size() - 2; i > 0; --i)
    {
        if (grid[i][c] > max)
        {
            max = grid[i][c];
            if (map[i][c] == 0)
            {
                ++sum;
                map[i][c] = 1;
            }
        }
    }

    return sum;
}

size_t score(const Grid& grid, const size_t r, const size_t c)
{
    size_t leftScore = 0;
    for (size_t i = c - 1; i < grid.size(); --i)
    {
        ++leftScore;
        if (grid[r][i] >= grid[r][c])
        {
            break;
        }
    }

    size_t rightScore = 0;
    for (size_t i = c + 1; i < grid[r].size(); ++i)
    {
        ++rightScore;
        if (grid[r][i] >= grid[r][c])
        {
            break;
        }
    }

    size_t upScore = 0;
    for (size_t i = r - 1; i < grid.size(); --i)
    {
        ++upScore;
        if (grid[i][c] >= grid[r][c])
        {
            break;
        }
    }

    size_t downScore = 0;
    for (size_t i = r + 1; i < grid.size(); ++i)
    {
        ++downScore;
        if (grid[i][c] >= grid[r][c])
        {
            break;
        }
    }

    return leftScore * rightScore * upScore * downScore;
}

size_t solvePart1(const Grid& grid)
{
    Grid map(grid.size(), Row(grid[0].size(), {}));

    size_t sum = grid.size() * 2 + grid[0].size() * 2 - 4;

    for (size_t i = 1; i < grid.size() - 1; ++i)
    {
        sum += solveRow(grid, map, i);
        sum += solveCol(grid, map, i);
    }

    return sum;
}

size_t solvePart2(const Grid& grid)
{
    size_t max = 0;

    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid.size(); ++j)
        {
            max = std::max(max, score(grid, i, j));
        }
    }

    return max;
}
}

TEST_CASE("2022-day-08")
{
    std::stringstream ss;
    ss << "30373" << std::endl
       << "25512" << std::endl
       << "65332" << std::endl
       << "33549" << std::endl
       << "35390" << std::endl;

    const auto grid = parse(ss);
    REQUIRE(grid.size() == 5);
    REQUIRE(grid[0].size() == 5);
    REQUIRE(grid[1].size() == 5);
    REQUIRE(grid[2].size() == 5);
    REQUIRE(grid[3].size() == 5);
    REQUIRE(grid[4].size() == 5);

    REQUIRE(grid[0][0] == 3);
    REQUIRE(grid[0][1] == 0);
    REQUIRE(grid[0][2] == 3);
    REQUIRE(grid[0][3] == 7);
    REQUIRE(grid[0][4] == 3);

    REQUIRE(grid[1][0] == 2);
    REQUIRE(grid[1][1] == 5);
    REQUIRE(grid[1][2] == 5);
    REQUIRE(grid[1][3] == 1);
    REQUIRE(grid[1][4] == 2);

    REQUIRE(grid[2][0] == 6);
    REQUIRE(grid[2][1] == 5);
    REQUIRE(grid[2][2] == 3);
    REQUIRE(grid[2][3] == 3);
    REQUIRE(grid[2][4] == 2);

    REQUIRE(grid[3][0] == 3);
    REQUIRE(grid[3][1] == 3);
    REQUIRE(grid[3][2] == 5);
    REQUIRE(grid[3][3] == 4);
    REQUIRE(grid[3][4] == 9);

    REQUIRE(grid[4][0] == 3);
    REQUIRE(grid[4][1] == 5);
    REQUIRE(grid[4][2] == 3);
    REQUIRE(grid[4][3] == 9);
    REQUIRE(grid[4][4] == 0);

    REQUIRE(solvePart1(grid) == 21);
    REQUIRE(solvePart2(grid) == 8);
}

void solution()
{
    const auto grid = parse(std::cin);
    std::cout << solvePart1(grid) << std::endl;
    std::cout << solvePart2(grid) << std::endl;
}
}
