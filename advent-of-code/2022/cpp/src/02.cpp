#include "02.hpp"

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>

namespace aoc02
{
namespace
{
using T = int64_t;
enum class Item : T { Rock = 0, Paper = 1, Scissor = 2 };
enum class Outcome : T { Loss, Draw, Win };
using Data = std::vector<std::pair<Item, Item>>;

constexpr T draw = 3;
constexpr T win = 6;

Item itemFromChar(const char itemName)
{
    switch (itemName)
    {
    case 'A':
    case 'X':
        return Item::Rock;
    case 'B':
    case 'Y':
        return Item::Paper;
    case 'C':
    case 'Z':
        return Item::Scissor;
    }
    throw std::runtime_error("Unknown item: '" + std::string(itemName, 1) + "'");
}

char itemToChar(const Item item, const std::vector<char>& names)
{
    switch(item)
    {
    case Item::Rock : return names[0];
    case Item::Paper : return names[1];
    case Item::Scissor : return names[2];
    }
    throw std::runtime_error("Unknown item: '" + std::to_string(static_cast<T>(item)) + "'");
}

char opponentToToChar(const Item item)
{
    return itemToChar(item, {'A', 'B', 'C'});
}

char playerToChar(const Item item)
{
    return itemToChar(item, {'X', 'Y', 'Z'});
}

Outcome outcomeFromItem(const Item item)
{
    switch (item)
    {
    case Item::Rock:
        return Outcome::Loss;
    case Item::Paper:
        return Outcome::Draw;
    case Item::Scissor:
        return Outcome::Win;
    }
    throw std::runtime_error("Unknown item: '" + std::to_string(static_cast<T>(item)) + "'");
}

Data parse(std::istream &is)
{
    Data data;

    char opponent, player;
    while (1)
    {
        is >> opponent >> player;
        if (is.eof())
        {
            break;
        }

        data.push_back({itemFromChar(opponent), itemFromChar(player)});
    }

    //std::cout << "data.size=" << data.size() << std::endl;

    return data;
}

T solveStep(const Item opponent, const Item player)
{
    const auto opponentVal = static_cast<T>(opponent);
    const auto playerVal = static_cast<T>(player);

    T score = playerVal + 1;

    if (opponent == player)
    {
        score += draw;
    }
    else if ((opponentVal + 1 ) % 3 == playerVal)
    {
        score += win;
    }

    return score;
}

Item selectItem(const Item opponent, const Outcome outcome)
{
    const auto opponentVal = static_cast<T>(opponent);

    if (outcome == Outcome::Loss)
    {
        return Item{(opponentVal + 2) % 3};
    }
    else if (outcome == Outcome::Draw)
    {
        return opponent;
    }
    else if (outcome == Outcome::Win)
    {
        return Item{(opponentVal + 1) % 3};
    }
    throw std::runtime_error("Unknown outcome: '" + std::to_string(static_cast<T>(outcome)) + "'");
}

T solvePart1(const Data& data)
{
    T score = 0;
    for (const auto& [opponent, player] : data)
    {
        const auto stepScore = solveStep(opponent, player);
        //std::cout << opponentToToChar(opponent) << " " << playerToChar(player) << " = " << stepScore << std::endl;
        score += stepScore;
    }
    return score;
}

T solvePart2(const Data& data)
{
    T score = 0;
    for (const auto& [opponent, player] : data)
    {
        const auto outcome = outcomeFromItem(player);
        const auto stepScore = solveStep(opponent, selectItem(opponent, outcome));
        //std::cout << opponentToToChar(opponent) << " " << playerToChar(player) << " = " << stepScore << std::endl;
        score += stepScore;
    }
    return score;
}
}

TEST_CASE("2022-day-02")
{
    assert(solveStep(Item::Rock, Item::Rock) == 4);
    assert(solveStep(Item::Rock, Item::Paper) == 8);
    assert(solveStep(Item::Rock, Item::Scissor) == 3);

    assert(solveStep(Item::Paper, Item::Rock) == 1);
    assert(solveStep(Item::Paper, Item::Paper) == 5);
    assert(solveStep(Item::Paper, Item::Scissor) == 9);

    assert(solveStep(Item::Scissor, Item::Rock) == 7);
    assert(solveStep(Item::Scissor, Item::Paper) == 2);
    assert(solveStep(Item::Scissor, Item::Scissor) == 6);

    assert(selectItem(Item::Rock, Outcome::Loss) == Item::Scissor);
    assert(selectItem(Item::Rock, Outcome::Draw) == Item::Rock);
    assert(selectItem(Item::Rock, Outcome::Win) == Item::Paper);

    assert(selectItem(Item::Paper, Outcome::Loss) == Item::Rock);
    assert(selectItem(Item::Paper, Outcome::Draw) == Item::Paper);
    assert(selectItem(Item::Paper, Outcome::Win) == Item::Scissor);

    assert(selectItem(Item::Scissor, Outcome::Loss) == Item::Paper);
    assert(selectItem(Item::Scissor, Outcome::Draw) == Item::Scissor);
    assert(selectItem(Item::Scissor, Outcome::Win) == Item::Rock);

    std::stringstream ss;
    ss << "A Y\nB X\nC Z";

    const auto data = parse(ss);

    assert(solvePart1(data) == 15);
    assert(solvePart2(data) == 12);
}

void solution()
{
    const auto data = parse(std::cin);
    std::cout << solvePart1(data) << std::endl;
    std::cout << solvePart2(data) << std::endl;
}
}
