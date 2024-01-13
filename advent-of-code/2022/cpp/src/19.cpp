#include "19.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <stack>
#include <sstream>
#include <vector>

namespace aoc19
{
namespace
{
enum class ResourceType : uint8_t { Ore = 0, Clay = 1, Obsidian = 2, Geode = 3 };

size_t toi(const ResourceType type)
{
    return static_cast<size_t>(type);
}

ResourceType fromI(const size_t idx)
{
    return static_cast<ResourceType>(idx);
}

static constexpr size_t ResourceTypesCount = 4;

using Resources = std::array<uint32_t, ResourceTypesCount>;

std::ostream& operator<<(std::ostream& os, const Resources& r)
{
    return os << "{ore=" << r[toi(ResourceType::Ore)] << ",clay=" << r[toi(ResourceType::Clay)]
              << ",obsidian=" << r[toi(ResourceType::Obsidian)] << ",geode=" << r[toi(ResourceType::Geode)] << "}";
}

bool isEnough(const Resources& resources, const Resources& robotCost)
{
    return resources[0] >= robotCost[0] && resources[1] >= robotCost[1] && resources[2] >= robotCost[2] && resources[3] >= robotCost[3];
}

Resources& operator-=(Resources& resources, const Resources& robotCost)
{
    if (resources < robotCost)
    {
        std::stringstream ss;
        ss << "Not enough resources: " << resources << " to build a robot: " << robotCost;
        throw std::runtime_error(ss.str());
    }
    resources[toi(ResourceType::Ore)] -= robotCost[toi(ResourceType::Ore)];
    resources[toi(ResourceType::Clay)] -= robotCost[toi(ResourceType::Clay)];
    resources[toi(ResourceType::Obsidian)] -= robotCost[toi(ResourceType::Obsidian)];
    return resources;
}

Resources fromIs(std::istream& is)
{
    uint32_t ore = 0, clay = 0, obsidian = 0;

    std::string token;
    is >> token >> token >> token >> token;
    is >> ore;
    is >> token;

    bool isNext = true;
    if (token.back() == '.')
    {
        isNext = false;
        token.pop_back();
    }

    if (isNext)
    {
        is >> token;
        is >> clay;
        is >> token;
        token.pop_back();
        if (token == "obsidian")
        {
            std::swap(clay, obsidian);
        }
    }

    return {ore, clay, obsidian, 0};
}

struct Blueprint
{
    using RobotCosts = std::array<Resources, ResourceTypesCount>;

    Blueprint(const uint32_t idx, const RobotCosts& robotCosts);

    const uint32_t idx;
    const RobotCosts robotCosts;
    const Resources limits;

private:
    Resources buildLimits() const;
};

std::ostream& operator<<(std::ostream& os, const Blueprint& b)
{
    return os << "Blueprint " << b.idx << ": "
              << "Each ore robot costs " << b.robotCosts[toi(ResourceType::Ore)][toi(ResourceType::Ore)] << " ore. "
              << "Each clay robot costs " << b.robotCosts[toi(ResourceType::Clay)][toi(ResourceType::Ore)] << " ore. "
              << "Each obsidian robot costs " << b.robotCosts[toi(ResourceType::Obsidian)][toi(ResourceType::Ore)] << " ore and "
              << b.robotCosts[toi(ResourceType::Obsidian)][toi(ResourceType::Clay)] << " clay."
              << "Each geode robot costs " << b.robotCosts[toi(ResourceType::Geode)][toi(ResourceType::Ore)] << " ore and "
              << b.robotCosts[toi(ResourceType::Geode)][toi(ResourceType::Obsidian)] << " obsidian.";
}

bool operator==(const Blueprint& lhs, const Blueprint& rhs)
{
    return lhs.idx == rhs.idx && lhs.robotCosts == rhs.robotCosts;
}

Resources Blueprint::buildLimits() const
{
    Resources limits;
    limits[toi(ResourceType::Ore)] = std::max({robotCosts[0][toi(ResourceType::Ore)], robotCosts[1][toi(ResourceType::Ore)], robotCosts[2][toi(ResourceType::Ore)], robotCosts[3][toi(ResourceType::Ore)]});
    limits[toi(ResourceType::Clay)] = std::max({robotCosts[0][toi(ResourceType::Clay)], robotCosts[1][toi(ResourceType::Clay)], robotCosts[2][toi(ResourceType::Clay)], robotCosts[3][toi(ResourceType::Clay)]});
    limits[toi(ResourceType::Obsidian)] = std::max({robotCosts[0][toi(ResourceType::Obsidian)], robotCosts[1][toi(ResourceType::Obsidian)], robotCosts[2][toi(ResourceType::Obsidian)], robotCosts[3][toi(ResourceType::Obsidian)]});
    limits[toi(ResourceType::Obsidian)] = std::numeric_limits<uint32_t>::max();
    return limits;
}

Blueprint::Blueprint(const uint32_t idx, const RobotCosts& robotCosts)
    : idx(idx)
    , robotCosts(robotCosts)
    , limits(buildLimits())
{}

using Blueprints = std::vector<Blueprint>;

Blueprints parse(std::istream& is)
{
    Blueprints blueprints;

    std::string token;
    uint32_t idx;

    while(1)
    {
        is >> token;
        if (is.eof())
        {
            break;
        }

        is >> idx >> token;

        const auto robotCosts1 = fromIs(is);
        const auto robotCosts2 = fromIs(is);
        const auto robotCosts3 = fromIs(is);
        const auto robotCosts4 = fromIs(is);

        blueprints.emplace_back(idx, Blueprint::RobotCosts{robotCosts1, robotCosts2, robotCosts3, robotCosts4});
    }

    return blueprints;
}

struct State
{
    Resources robotCounts = {};
    Resources resources = {};
    uint32_t timestamp = 0;
    uint32_t potentialMaxGeode = 0;
    std::string ident;

    void calculatePotentialMax(const uint32_t limit);
};

void State::calculatePotentialMax(const uint32_t limit)
{
    const auto n = limit - timestamp;
    const auto currentlyAchievable = robotCounts[toi(ResourceType::Geode)] * n;
    // https://en.wikipedia.org/wiki/Triangular_number
    potentialMaxGeode = (n * (n + 1)) / 2;
    potentialMaxGeode += currentlyAchievable;
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
    return os << std::string(state.timestamp, '-') << "t=" << state.timestamp << ",res=" << state.resources
              << ",rob=" << state.robotCounts << ",pot=" << state.potentialMaxGeode << "," << state.ident;
}

class StateGenerator
{
public:

    StateGenerator(const Blueprint& blueprint, const uint32_t limit);

    State init(const uint32_t limit) const;
    std::optional<State> tryBuildRobot(const State& current, const ResourceType type) const;
    State collectOnly(const State& current) const;

private:
    using Modifier = std::function<void(State&)>;

    void collect(State& state) const;

    const Blueprint& blueprint;
    uint8_t limit;
};

StateGenerator::StateGenerator(const Blueprint& blueprint, const uint32_t limit)
    : blueprint(blueprint)
    , limit(limit)
{}

State StateGenerator::init(const uint32_t limit) const
{
    State init;
    init.robotCounts[toi(ResourceType::Ore)] = 1;
    init.calculatePotentialMax(limit);
    return init;
}

std::optional<State> StateGenerator::tryBuildRobot(const State& current, const ResourceType type) const
{
    auto newState = current;

    const auto typeIdx = toi(type);

    const auto& robotCost = blueprint.robotCosts[typeIdx];

    if (!isEnough(current.resources, robotCost))
    {
        return std::nullopt;
    }

    auto& robotsCount = newState.robotCounts[typeIdx];

    newState.resources -= robotCost;
    collect(newState);
    ++robotsCount;

    ++newState.timestamp;
    newState.ident = "br" + std::to_string(toi(type));
    newState.calculatePotentialMax(limit);
    return newState;
}

void StateGenerator::collect(State& state) const
{
    state.resources[toi(ResourceType::Ore)] += state.robotCounts[toi(ResourceType::Ore)];
    state.resources[toi(ResourceType::Clay)] += state.robotCounts[toi(ResourceType::Clay)];
    state.resources[toi(ResourceType::Obsidian)] += state.robotCounts[toi(ResourceType::Obsidian)];
    state.resources[toi(ResourceType::Geode)] += state.robotCounts[toi(ResourceType::Geode)];
}

State StateGenerator::collectOnly(const State& current) const
{
    auto newState = current;
    collect(newState);
    ++newState.timestamp;
    newState.calculatePotentialMax(limit);
    newState.ident = "c";
    return newState;
}

//#define D

#ifdef D
#define DEBUG(msg) std::cout << msg;
#else
#define DEBUG(msg);
#endif

uint32_t solve(const Blueprint& blueprint, const uint32_t limit)
{
    DEBUG(blueprint << std::endl << std::endl);

    StateGenerator stateGen(blueprint, limit);

    uint32_t geodeMax = 0;

    std::stack<State> s;
    s.push(stateGen.init(limit));

    while (!s.empty())
    {
        const auto current = s.top();
        s.pop();

        DEBUG(current << ": ");

        if (current.timestamp > limit)
        {
            throw std::runtime_error("Time limit of '" + std::to_string(limit) + "' exceeded: '" + std::to_string(current.timestamp) + "'");
        }
        if (current.timestamp == limit)
        {
            DEBUG("geodeMax=" << geodeMax << ", current=" << current.resources[toi(ResourceType::Geode)] << std::endl);
            geodeMax = std::max(geodeMax, current.resources[toi(ResourceType::Geode)]);
            continue;
        }

        if (current.resources[toi(ResourceType::Geode)] + current.potentialMaxGeode <= geodeMax)
        {
            DEBUG("nopot " << current.resources[toi(ResourceType::Geode)] << " + " << current.potentialMaxGeode << " <= " << geodeMax << std::endl);
            continue;
        }

        if (const auto newState = stateGen.tryBuildRobot(current, ResourceType::Geode);
            newState.has_value())
        {
            DEBUG(">br1" << std::endl);
            s.push(newState.value());
            continue;
        }

        for (size_t i = 0; i < ResourceTypesCount - 1; ++i)
        {
            if (current.robotCounts[i] < blueprint.limits[i])
            {
                if (const auto newState = stateGen.tryBuildRobot(current, fromI(i));
                    newState.has_value())
                {
                    DEBUG(">br" << i << " ");
                    s.push(newState.value());
                }
            }
        }

        DEBUG(">c" << std::endl);
        s.push(stateGen.collectOnly(current));
    }

    DEBUG(geodeMax << std::endl);
    return geodeMax;
}

size_t solvePart1(const Blueprints& blueprints)
{
    static constexpr uint32_t limit = 24;
    size_t result = 0;

    for (const auto& blueprint : blueprints)
    {
        const auto maxGeodes = solve(blueprint, limit);
        std::cout << "id=" << blueprint.idx << ", max=" << maxGeodes << std::endl;
        result += blueprint.idx * maxGeodes;
    }

    return result;
}

size_t solvePart2(const Blueprints& blueprints)
{
    static constexpr uint32_t limit = 32;
    size_t result = 1;

    for (size_t i = 0; i < 3; ++i)
    {
        const auto maxGeodes = solve(blueprints[i], limit);
        std::cout << "id=" << blueprints[i].idx << ", max=" << maxGeodes << std::endl;
        result *= maxGeodes;
    }

    return result;
}
}

TEST_CASE("2022-day-19-small")
{
    auto input = Utility::getTestData("19/test-input1.txt");
    const auto blueprints = parse(input);

    static constexpr uint32_t limit = 7;

    REQUIRE(solve(blueprints[0], limit) == 1);
}

TEST_CASE("2022-day-19")
{
    auto input = Utility::getTestData("19/test-input.txt");
    const auto blueprints = parse(input);

    REQUIRE(blueprints.size() == 2);
    REQUIRE(blueprints[0] == Blueprint(1, {Resources{4, 0, 0, 0}, Resources{2, 0, 0, 0}, Resources{3, 14, 0, 0}, Resources{2, 0, 7, 0}}));
    REQUIRE(blueprints[1] == Blueprint(2, {Resources{2, 0, 0, 0}, Resources{3, 0, 0, 0}, Resources{3, 8, 0, 0}, Resources{3, 0, 12, 0}}));

    Resources res{3, 2, 1};
    Resources cost{3, 2, 1, 0};
    REQUIRE(isEnough(res, cost));

    --res[toi(ResourceType::Ore)];
    REQUIRE(!isEnough(res, cost));

    //static constexpr uint32_t limit1 = 24;

    //REQUIRE(solve(blueprints[0], limit1) == 9);
    //REQUIRE(solve(blueprints[1], limit1) == 12);

    //static constexpr uint32_t limit2 = 32;
    //REQUIRE(solve(blueprints[0], limit2) == 56);
    //REQUIRE(solve(blueprints[1], limit2) == 62);
}

void solution()
{
    const auto blueprints = parse(std::cin);
    std::cout << solvePart1(blueprints) << std::endl;
    std::cout << solvePart2(blueprints) << std::endl;
}
}
