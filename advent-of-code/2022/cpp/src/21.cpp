#include "21.hpp"

#include "Utility.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <sstream>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

namespace aoc21
{
namespace
{
struct Op
{
    std::string lhs, rhs;
    char op;
};

using T = int64_t;

struct Node
{
    bool leadsToHuman = false;
    T val;
    std::variant<Op, T> monkey;

    template <typename MonkeyT>
    bool has() const
    {
        return std::holds_alternative<MonkeyT>(monkey);
    }

    template <typename MonkeyT>
    MonkeyT get() const
    {
        return std::get<MonkeyT>(monkey);
    }
};


using Map = std::unordered_map<std::string, Node>;

std::optional<T> getIfNumber(const std::string& str)
{
    try
    {
        return std::stoll(str);
    }
    catch (...)
    {
    }
    return std::nullopt;
}

std::istream& operator>>(std::istream& is, Map& map)
{
    std::string token1, token2, token3, token4;
    while (1)
    {
        is >> token1;
        if (is.eof())
        {
            break;
        }

        token1.pop_back();
        is >> token2;
        if (const auto opt = getIfNumber(token2); opt.has_value())
        {
            map[token1].monkey = opt.value();
            continue;
        }
        is >> token3 >> token4;
        map[token1].monkey = Op{token2, token4, token3[0]};
    }
    return is;
}

T execOp(const T lhs, const T rhs, const char op)
{
    switch(op)
    {
    case '+': return lhs + rhs;
    case '-': return lhs - rhs;
    case '*': return lhs * rhs;
    case '/': return lhs / rhs;
    }
    throw std::runtime_error("Unknown operation: '" + std::string(1, op) + "'");
}

constexpr std::string_view humanSymbol = "humn";
const std::string rootSymbol = "root";

//#define D
#ifdef D
#define DEBUG(msg) std::cout << msg << std::endl;
#else
#define DEBUG(msg)
#endif

void resolve(Map& map, const std::string& symbol)
{
    if (map.at(symbol).has<T>())
    {
        if (symbol == humanSymbol)
        {
            map.at(symbol).leadsToHuman = true;
        }
        DEBUG(symbol << ": " << map.at(symbol).get<T>());
        map.at(symbol).val = map.at(symbol).get<T>();
        return;
    }
    const auto op = map.at(symbol).get<Op>();
    resolve(map, op.lhs);
    const auto val1 = map.at(op.lhs).val;
    if (map.at(op.lhs).leadsToHuman)
    {
        map.at(symbol).leadsToHuman = true;
    }

    resolve(map, op.rhs);
    const auto val2 = map.at(op.rhs).val;
    if (map.at(op.rhs).leadsToHuman)
    {
        map.at(symbol).leadsToHuman = true;
    }

    DEBUG(symbol << ": " << op.lhs << "=" << val1 << " " << op.op << " " << op.rhs << "=" << val2 << " = " << execOp(val1, val2, op.op));
    map.at(symbol).val = execOp(val1, val2, op.op);
}

T findVal(const Map& map, const std::string& symbol, const T target)
{
    if (symbol == humanSymbol)
    {
        return target;
    }
    T reverseOpRes;

    const auto& op = map.at(symbol).get<Op>();

    if (map.at(op.lhs).leadsToHuman)
    {
        const auto val = map.at(op.rhs).val;
        switch (op.op)
        {
            case '+': reverseOpRes = target - val;  break;
            case '-': reverseOpRes = target + val;  break;
            case '*': reverseOpRes = target / val;  break;
            case '/': reverseOpRes = target * val;  break;
        }
        return findVal(map, op.lhs, reverseOpRes);
    }
    else if (map.at(op.rhs).leadsToHuman)
    {
        const auto val = map.at(op.lhs).val;
        switch (op.op)
        {
            case '+': reverseOpRes = target - val;  break;
            case '-': reverseOpRes = val - target;  break;
            case '*': reverseOpRes = target / val;  break;
            case '/': reverseOpRes = val / target;  break;
        }
        return findVal(map, op.rhs, reverseOpRes);
    }
    throw std::runtime_error("No path from '" + symbol +"' leads to human");
}

T solvePart1(Map& map)
{
    resolve(map, rootSymbol);
    return map.at(rootSymbol).val;
}

T solvePart2(const Map& map)
{
    const auto op = map.at(rootSymbol).get<Op>();

    const auto& lhs = map.at(op.lhs);
    const auto& rhs = map.at(op.rhs);

    if (lhs.leadsToHuman)
    {
        return findVal(map, op.lhs, rhs.val);
    }
    return findVal(map, op.rhs, lhs.val);
}

void requireOp(const Map& map, const std::string& key, const Op& op)
{
    REQUIRE(map.at(key).has<Op>());
    REQUIRE(map.at(key).get<Op>().lhs == op.lhs);
    REQUIRE(map.at(key).get<Op>().rhs == op.rhs);
    REQUIRE(map.at(key).get<Op>().op == op.op);
}

void requireVal(const Map& map, const std::string& key, const T val)
{
    REQUIRE(map.at(key).has<T>());
    REQUIRE(map.at(key).get<T>() == val);
}
}

TEST_CASE("2022-day-21")
{
    auto input = Utility::getTestData("21/test-input.txt");
    Map map;
    input >> map;

    REQUIRE(map.size() == 15);
    requireOp(map, "root", {"pppw", "sjmn", '+'});
    requireVal(map, "dbpl", 5);
    requireOp(map, "cczh", {"sllz", "lgvd", '+'});
    requireVal(map, "zczc", 2);
    requireOp(map, "ptdq", {"humn", "dvpt", '-'});
    requireVal(map, "dvpt", 3);
    requireVal(map, "lfqf", 4);
    requireVal(map, "humn", 5);
    requireVal(map, "ljgn", 2);
    requireOp(map, "sjmn", {"drzm", "dbpl", '*'});
    requireVal(map, "sllz", 4);
    requireOp(map, "pppw", {"cczh", "lfqf", '/'});
    requireOp(map, "lgvd", {"ljgn", "ptdq", '*'});
    requireOp(map, "drzm", {"hmdt", "zczc", '-'});
    requireVal(map, "hmdt", 32);

    REQUIRE(solvePart1(map) == 152);
    REQUIRE(solvePart2(map) == 301);
}

void solution()
{
    Map map;
    std::cin >> map;
    std::cout << solvePart1(map) << std::endl;
    std::cout << solvePart2(map) << std::endl;
}
}
