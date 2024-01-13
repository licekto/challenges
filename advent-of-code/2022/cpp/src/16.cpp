#include "16.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace Utility;

namespace aoc16
{
namespace
{
struct Valve
{
    std::string name;
    T rate;
    std::vector<std::string> leadsToStr;
    std::vector<size_t> leadsTo;

    static Valve fromStr(const std::string& str);
};

Valve Valve::fromStr(const std::string& str)
{
    Valve valve;

    valve.name = str.substr(6, 2);
    const auto rateBegin = str.find_first_of('=') + 1;
    const auto rateSize = str.find_first_of(';') - rateBegin;
    const auto rateStr = str.substr(rateBegin, rateSize);
    valve.rate = std::stoll(rateStr);

    auto valvePos = str.find("valve") + 5;
    if (str[valvePos] == 's')
    {
        valvePos += 2;
    }
    else
    {
        ++valvePos;
    }
    const auto leadsTo = str.substr(valvePos);

    auto it = std::begin(leadsTo);
    while (1)
    {
        std::string to(2, 0);
        to[0] = *it;
        ++it;
        to[1] = *it;
        ++it;

        valve.leadsToStr.push_back(to);

        if (it == std::end(leadsTo))
        {
            break;
        }
        ++it;
        ++it;
    }

    return valve;
}

constexpr size_t maxValves = 70;

using Valves = std::vector<Valve>;

Valves parse(std::istream& is)
{
    Valves valves;

    std::string line;
    while (std::getline(is, line))
    {
        valves.push_back(Valve::fromStr(line));
    }

    std::sort(std::begin(valves), std::end(valves), [](const Valve& v1, const Valve& v2){ return v1.name < v2.name; });

    for (size_t i = 0; i < valves.size(); ++i)
    {
        for (const auto& neighStr : valves[i].leadsToStr)
        {
            for (size_t j = 0; j < valves.size(); ++j)
            {
                if (valves[j].name == neighStr)
                {
                    valves[i].leadsTo.push_back(j);
                    break;
                }
            }
        }
        std::sort(std::begin(valves[i].leadsTo), std::end(valves[i].leadsTo));
    }

    return valves;
}

using Distances = std::vector<std::vector<size_t>>;

Distances floydWarshall(const Valves& valves)
{
    static constexpr size_t maxVal  = std::numeric_limits<size_t>::max() / 10;
    Distances distances(valves.size(), std::vector<size_t>(valves.size(), maxVal));

    for (size_t i = 0; i < valves.size(); ++i)
    {
        for (const auto& neigh : valves[i].leadsTo)
        {
            distances[i][neigh] = 1;
        }
        distances[i][i] = 0;
    }

    for (size_t k = 0; k < valves.size(); ++k)
    {
        for (size_t i = 0; i < valves.size(); ++i)
        {
            for (size_t j = 0; j < valves.size(); ++j)
            {
                distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }

    return distances;
}

using ValveSet = std::bitset<maxValves>;

struct Path
{
    Path(const Valves& valves)
        : valves(valves)
    {}

    const Valves& valves;
    size_t count = 0;
    uint8_t openValves[maxValves];
    size_t pressure = 0;
};

bool areDisjoint(const Path& p1, const Path& p2)
{
    for (size_t i = 0; i < p1.count; ++i)
    {
        for (size_t j = 0; j < p2.count; ++j)
        {
            if (p1.openValves[i] == p2.openValves[j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator==(const Path& p1, const Path& p2)
{
    if (p1.count != p2.count || p1.pressure != p2.pressure)
    {
        return false;
    }
    for (size_t i = 0; i < p1.count; ++i)
    {
        if (p1.openValves[i] != p2.openValves[i])
        {
            return false;
        }
    }
    return true;
}

struct PathHash
{
    uint64_t operator()(const Path& path) const
    {
        std::string str;

        for (size_t i = 0; i < path.count; ++i)
        {
            str.append(path.valves.at(path.openValves[i]).name);
        }

        return std::hash<std::string>{}(str);
    }
};

using PathSet = std::unordered_set<Path, PathHash>;

struct State
{
    uint8_t openedAt[maxValves] = {};
    uint8_t valve = 0, minutesElapsed = 0, valvesCount;

    size_t calculatePressure(const Valves& valves, const size_t minutesLimit) const
    {
        size_t pressure = 0;
        for (size_t i = 0; i < valvesCount; ++i)
        {
            if (openedAt[i])
            {
                pressure += (minutesLimit - openedAt[i]) * valves.at(i).rate;
            }
        }

        return pressure;
    }

    std::string print(const Valves& valves) const
    {
        std::stringstream ss;
        for (size_t i = 0; i < valvesCount; ++i)
        {
            if (openedAt[i])
            {
                ss << valves.at(i).name << "[" << i << "]" << ":at=" << (int)openedAt[i] << ", ";
            }
        }
        return ss.str();
    }

    bool allValvesOpen(const ValveSet& valveSet)
    {
        for (size_t i = 0; i < valvesCount; ++i)
        {
            if (valveSet[i] && !openedAt[i])
            {
                return false;
            }
        }
        return true;
    }

    Path buildPath(const Valves& valves, const size_t minutesLimit) const
    {
        std::vector<std::pair<size_t, size_t>> minToValve;

        for (size_t i = 0; i < valvesCount; ++i)
        {
            if (openedAt[i])
            {
                minToValve.emplace_back(openedAt[i], i);
            }
        }

        std::sort(std::begin(minToValve), std::end(minToValve));

        Path path(valves);

        path.count = minToValve.size();
        for (size_t i = 0; i < minToValve.size(); ++i)
        {
            path.openValves[i] = minToValve[i].second;
        }

        path.pressure = calculatePressure(valves, minutesLimit);

        return path;
    }
};

ValveSet getValvesToOpen(const Valves& valves)
{
    ValveSet valvesToOpen;
    for (size_t i = 0; i < valves.size(); ++i)
    {
        if (valves[i].rate != 0)
        {
            valvesToOpen[i] = true;
        }
    }
    return valvesToOpen;
}

PathSet findMaxPressure(const Valves& valves, const Distances& distances, const size_t minutesLimit)
{
    size_t maxPressure = 0;
    const auto valvesToOpen = getValvesToOpen(valves);

    State init;
    init.valvesCount = valves.size();
    std::stack<State> stack;
    stack.push(init);

    PathSet pathSet;

    while (!stack.empty())
    {
        auto currentState = stack.top();
        stack.pop();

        if (currentState.minutesElapsed < minutesLimit && valves.at(currentState.valve).rate != 0)
        {
            ++currentState.minutesElapsed;
            currentState.openedAt[currentState.valve] = currentState.minutesElapsed;
        }

        const auto currentPressure = currentState.calculatePressure(valves, minutesLimit);

        const auto path = currentState.buildPath(valves, minutesLimit);
        if (const auto it = pathSet.find(path); it != std::end(pathSet))
        {
            if (it->pressure < currentPressure)
            {
                pathSet.insert(path);
            }
        }
        else
        {
            pathSet.insert(path);
        }

        if (currentState.minutesElapsed >= minutesLimit || currentState.allValvesOpen(valvesToOpen))
        {
            maxPressure = std::max(maxPressure, currentPressure);
            continue;
        }

        for (size_t i = 0; i < currentState.valvesCount; ++i)
        {
            if (!valvesToOpen[i] || currentState.openedAt[i])
            {
                continue;
            }

            const auto distance = distances[currentState.valve][i];
            auto newState = currentState;
            newState.valve = i;
            newState.minutesElapsed += distance;

            stack.push(newState);
        }
    }

    return pathSet;
}

size_t solvePart1(const Valves& valves, const Distances& distances)
{
    const auto pathSet = findMaxPressure(valves, distances, 30);

    size_t maxPressure = 0;
    for (const auto& path : pathSet)
    {
        maxPressure = std::max(maxPressure, path.pressure);
    }
    return maxPressure;
}

size_t solvePart2(const Valves& valves, const Distances& distances)
{
    const auto pathSet = findMaxPressure(valves, distances, 26);

    size_t maxPressure = 0;
    for (auto it1 = std::begin(pathSet); it1 != std::end(pathSet); ++it1)
    {
        auto it2 = it1;
        it2 = std::next(it2);
        for (; it2 != std::end(pathSet); ++it2)
        {
            if (areDisjoint(*it1, *it2))
            {
                maxPressure = std::max(maxPressure, it1->pressure + it2->pressure);
            }
        }
    }
    return maxPressure;
}

void testValve(const Valves& valves, const size_t idx, const std::string& name, const T rate, const std::vector<std::string>& leadsTo)
{
    REQUIRE(valves.at(idx).name == name);
    REQUIRE(valves.at(idx).rate == rate);

    for (size_t i = 0; i < leadsTo.size(); ++i)
    {
        REQUIRE(valves.at(idx).leadsToStr[i] == leadsTo[i]);
    }
}
}

TEST_CASE("2022-day-16")
{
    auto input = Utility::getTestData("16/test-input.txt");
    const auto valves = parse(input);

    REQUIRE(valves.size() == 10);

    testValve(valves, 0, "AA", 0, {"DD", "II", "BB"});
    testValve(valves, 1, "BB", 13, {"CC", "AA"});
    testValve(valves, 2, "CC", 2, {"DD", "BB"});
    testValve(valves, 3, "DD", 20, {"CC", "AA", "EE"});
    testValve(valves, 4, "EE", 3, {"FF", "DD"});
    testValve(valves, 5, "FF", 0, {"EE", "GG"});
    testValve(valves, 6, "GG", 0, {"FF", "HH"});
    testValve(valves, 7, "HH", 22, {"GG"});
    testValve(valves, 8, "II", 0, {"AA", "JJ"});
    testValve(valves, 9, "JJ", 21, {"II"});

    const auto distances = floydWarshall(valves);
    const Distances expected =
    {
        {0, 1, 2, 1, 2, 3, 4, 5, 1, 2},
        {1, 0, 1, 2, 3, 4, 5, 6, 2, 3},
        {2, 1, 0, 1, 2, 3, 4, 5, 3, 4},
        {1, 2, 1, 0, 1, 2, 3, 4, 2, 3},
        {2, 3, 2, 1, 0, 1, 2, 3, 3, 4},
        {3, 4, 3, 2, 1, 0, 1, 2, 4, 5},
        {4, 5, 4, 3, 2, 1, 0, 1, 5, 6},
        {5, 6, 5, 4, 3, 2, 1, 0, 6, 7},
        {1, 2, 3, 2, 3, 4, 5, 6, 0, 1},
        {2, 3, 4, 3, 4, 5, 6, 7, 1, 0}
    };

    REQUIRE(distances == expected);
    REQUIRE(solvePart1(valves, distances) == 1651);
    REQUIRE(solvePart2(valves, distances) == 1707);
}

void solution()
{
    const auto valves = parse(std::cin);
    const auto distances = floydWarshall(valves);
    std::cout << solvePart1(valves, distances) << std::endl;
    std::cout << solvePart2(valves, distances) << std::endl;
}
}
