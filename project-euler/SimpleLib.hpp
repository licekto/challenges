#pragma once

#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <string.h>

// ANSI escape codes for text color
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#if defined(_WIN32) || defined(_WIN64)
#define DELIM_ '\\'
#else
#define DELIM_ '/'
#endif

inline std::string get_thread_name() {
    static constexpr size_t Len = 20;
    char buffer[Len] = {};
    pthread_getname_np(pthread_self(), buffer, Len);
    return buffer;
}

#define __FILENAME__ (strrchr(__FILE__, DELIM_) ? strrchr(__FILE__, DELIM_) + 1 : __FILE__)
#define source_info __FILENAME__ << "::" << __FUNCTION__ << "::" << __LINE__
//#define simple_log_this(this_ptr, msg) do {} while(0)
#define simple_log_this(this_ptr, msg) do { std::cout << "[" << get_thread_name() << "][" << source_info << "][this=" << this_ptr << "] - " << msg << "\n"; } while(0)
#define simple_log(msg) simple_log_this("-", msg)

namespace detail__
{
[[maybe_unused]] static bool TestsOk = true;
}

#define TEST_RESULT() do { std::cout << "Tests " << (detail__::TestsOk ? "OK" : "Failed") << std::endl; } while(0)
#define RESET_TEST_RESULT() do { detail__::TestsOk = false; } while(0)

#define ASSERT(cond) \
do \
{ \
    if (!(cond)) \
    { \
        std::cout << RED << source_info << " TestCase \"" << #cond << "\" failed: condition is not true" << RESET << std::endl; \
        detail__::TestsOk = false; \
    } \
} while(0)

#define ASSERT_EQ(a, b) \
do \
{ \
    const auto a_ = a; const auto b_ = b; \
    if (a_ != b_) \
    { \
        std::cout << RED << source_info << " TestCase \"" << #a << " == " << #b << "\" failed: '" << a_ << "' != '" << b_ << "'" << RESET << std::endl; \
        detail__::TestsOk = false; \
    } \
} while(0)

#define ASSERT_NEQ(a, b) \
do \
{ \
    const auto a_ = a; const auto b_ = b; \
    if (a_ == b_) \
    { \
        std::cout << RED << source_info << " TestCase \"" << #a << " != " << #b << "\" failed: '" << a_ << "' == '" << b_ << "'" << RESET << std::endl; \
        detail__::TestsOk = false; \
    } \
} while(0)

#define ASSERT_THROWS(exp, msg) \
do \
{ \
    bool thrown = false; \
    try \
    { \
        exp; \
    } \
    catch (const std::exception& ex) \
    { \
        assert(std::string(ex.what()), msg); \
        thrown = true; \
    } \
    if (!thrown) \
    { \
        std::cout << RED << source_info << " The \"" << #exp << "\" should have thrown an exception but it didn't" << RESET << std::endl; \
        detail__:TestsOk = false; \
    } \
} while(0)

struct Stats
{
    const double mean{0}, standardDeviation{0};
    std::string ToString(const std::string& units) const
    {
        std::stringstream ss;
        ss << "mean=" << mean << units << ", standardDeviation=" << standardDeviation << units;
        return ss.str();
    }
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

template <typename UnitT, typename F>
auto measure(const size_t iterations, const bool warmup, F&& f)
{
    std::vector<UnitT> measuredTimes;

    if (warmup)
    {
        for (size_t i = 0; i < iterations / 10; ++i)
        {
            std::forward<F>(f)();
        }
    }

    for (size_t i = 0; i < iterations; ++i)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        std::forward<F>(f)();
        const auto end = std::chrono::high_resolution_clock::now();
        measuredTimes.push_back(std::chrono::duration_cast<UnitT>(end - start));
    }

    return measuredTimes;
}
