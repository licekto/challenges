#pragma once

#include <chrono>
#include <cmath>
#include <fstream>
#include <map>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Utility
{
class CharMap
{
public:
    static uint64_t charToVal(const char c);
    static size_t Size();

    CharMap(const std::string_view& str);
    bool Contains(const char c) const;
    void Clear();

private:
    static constexpr size_t mapSize = 26 * 2 + 2;
    bool map[mapSize] = {};
};

template <typename UnitT, typename F>
auto measure(const size_t iterations, F&& f)
{
    std::vector<std::chrono::microseconds> measuredTimes;

    for (size_t i = 0; i < iterations; ++i)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::forward<F>(f)();
        const auto end = std::chrono::high_resolution_clock::now();
        measuredTimes.push_back(std::chrono::duration_cast<UnitT>(end - start));
    }

    return measuredTimes;
}

struct Stats
{
    const double mean{0}, standardDeviation{0};
};

template <typename UnitT>
Stats calculateStats(const std::vector<UnitT>& measuredTimes)
{
    double sum = 0.0;

    for (const auto& t : measuredTimes)
    {
        sum += t.count();
    }

    double mean = sum / measuredTimes.size();

    double standardDeviation = 0.0;

    for (const auto& t : measuredTimes)
    {
        standardDeviation += std::pow(t.count() - mean, 2);
    }

    standardDeviation = std::sqrt(standardDeviation / measuredTimes.size());

    return {mean, standardDeviation};
}

using T = int64_t;
struct Coord
{
    static constexpr T Min = std::numeric_limits<T>::min();
    static constexpr T Max = std::numeric_limits<T>::max();
    Coord() = default;
    Coord(const T x, const T y, const T z = 0);
    T x = 0, y = 0, z = 0;
};

bool operator==(const Coord& lhs, const Coord& rhs);
bool operator!=(const Coord& lhs, const Coord& rhs);
Coord operator+(const Coord& lhs, const Coord& rhs);
std::ostream& operator<<(std::ostream& os, const Coord& coord);

struct CoordHash
{
    uint64_t operator()(const Coord& coord) const;
};

using CoordSet = std::unordered_set<Coord, CoordHash>;

template <typename ValT>
using CoordMap = std::unordered_map<Coord, ValT, CoordHash>;

size_t manhatanDistance(const Coord& c1, const Coord& c2);

class RangeSet : public std::map<T, T>
{
public:
    void add(const T l, const T r);
};

std::ifstream getTestData(const std::string& filename);

void test();
}
