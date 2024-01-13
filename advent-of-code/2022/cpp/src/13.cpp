#include "13.hpp"

#include <catch2/catch_all.hpp>

#include "Utility.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace aoc13
{
namespace
{
using T = int64_t;

enum class Type { Value, List };

struct Item
{
    Item(const Type type)
        : type(type)
    {}

    virtual ~Item() = default;
    virtual void print(std::ostream& os) const = 0;

    template <typename T>
    const T *get() const
    {
        return dynamic_cast<const T*>(this);
    }

    Type type;
};

struct Value : public Item
{
    Value(const T value)
        : Item(Type::Value)
        , value(value)
    {}

    void print(std::ostream& os) const override
    {
        os << value;
    }

    T value;
};

struct List : public Item
{
    List()
        : Item(Type::List)
    {}

    void print(std::ostream& os) const override
    {
        os << "[";
        bool first = true;
        for (const auto& item : data)
        {
            if (!first)
            {
                os << ",";
            }
            first = false;
            item->print(os);
        }
        os << "]";
    }

    std::vector<std::unique_ptr<Item>> data;
};

enum class Cmp { Lt, Eq, Gt };

Cmp Compare(const Item* item1, const Item* item2)
{
    if (item1->type == Type::Value && item2->type == Type::Value)
    {
        const auto val1 = item1->get<Value>()->value;
        const auto val2 = item2->get<Value>()->value;

        if (val1 < val2)
        {
            return Cmp::Lt;
        }
        if (val1 == val2)
        {
            return Cmp::Eq;
        }
        if (val1 > val2)
        {
            return Cmp::Gt;
        }
    }

    if (item1->type == Type::List && item2->type == Type::List)
    {
        const auto& data1 = item1->get<List>()->data;
        const auto& data2 = item2->get<List>()->data;

        auto it1 = std::begin(data1);
        auto it2 = std::begin(data2);
        auto end1 = std::end(data1);
        auto end2 = std::end(data2);

        while (1)
        {
            if (it1 == end1 && it2 == end2)
            {
                return Cmp::Eq;
            }

            if (it1 == end1)
            {
                return Cmp::Lt;
            }
            if (it2 == end2)
            {
                return Cmp::Gt;
            }

            const auto cmp = Compare(it1->get(), it2->get());
            if (cmp == Cmp::Eq)
            {
                ++it1;
                ++it2;
                continue;
            }
            return cmp;
        }
    }

    if (item1->type == Type::List)
    {
        List tmp;
        tmp.data.push_back(std::make_unique<Value>(item2->get<Value>()->value));

        return Compare(item1, &tmp);
    }

    if (item2->type == Type::List)
    {
        List tmp;
        tmp.data.push_back(std::make_unique<Value>(item1->get<Value>()->value));

        return Compare(&tmp, item2);
    }

    throw std::runtime_error("Unknown condition");
}

std::ostream& operator<<(std::ostream& os, const List& list)
{
    list.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, List& list)
{
    if (is.peek() != '[')
    {
        throw std::runtime_error("The list must start with the '[' char");
    }
    is.get();

    while (1)
    {
        const char peek = is.peek();
        if (std::isdigit(peek))
        {
            T value;
            is >> value;
            auto val = std::make_unique<Value>(value);
            list.data.push_back(std::move(val));
            continue;
        }
        else if (peek == ',')
        {
            is.get();
            continue;
        }
        else if (peek == '[')
        {
            auto subList = std::make_unique<List>();
            is >> *subList;
            list.data.push_back(std::move(subList));
        }
        else if (peek == ']')
        {
            is.get();
            break;
        }
    }
    return is;
}

using Packets = std::vector<std::pair<List, List>>;

Packets parse(std::istream& is)
{
    Packets packets;

    std::string emptyLine;
    List list1, list2;
    while (1)
    {
        is >> list1;
        is.get();

        is >> list2;
        is.get();

        packets.push_back({std::move(list1), std::move(list2)});

        if (!std::getline(is, emptyLine))
        {
            break;
        }
    }

    return packets;
}

size_t solvePart1(const Packets& packets)
{
    size_t sum = 0, idx = 1;

    for (const auto& packet : packets)
    {
        const auto cmp = Compare(&packet.first, &packet.second);
        //std::cout << idx << ": ";
        if (cmp == Cmp::Eq || cmp == Cmp::Lt)
        {
            //std::cout << "true" << std::endl;
            sum += idx;
        }
        else
        {
            //std::cout << "false" << std::endl;
        }
        //std::cout << packet.first << std::endl << packet.second << std::endl << std::endl;
        ++idx;
    }

    return sum;
}

bool isDivider(const Item* item, const T dividerValue)
{
    if (item->type == Type::List)
    {
        const auto& data = item->get<List>()->data;
        if (data.size() == 1 && data[0]->type == Type::List)
        {
            const auto& innerData = data[0]->get<List>()->data;
            return innerData.size() == 1 && innerData[0]->type == Type::Value && innerData[0]->get<Value>()->value == dividerValue;
        }
    }
    return false;
}

size_t solvePart2(Packets&& packets)
{
    static constexpr T dividerValue1 = 2;
    static constexpr T dividerValue2 = 6;

    std::vector<List> sorted;

    auto list = std::make_unique<List>();

    List divider1;
    list->data.push_back(std::make_unique<Value>(dividerValue1));
    divider1.data.push_back(std::move(list));

    List divider2;
    list = std::make_unique<List>();
    list->data.push_back(std::make_unique<Value>(dividerValue2));
    divider2.data.push_back(std::move(list));

    sorted.push_back(std::move(divider1));
    sorted.push_back(std::move(divider2));

    for (auto&& packet : packets)
    {
        sorted.push_back(std::move(packet.first));
        sorted.push_back(std::move(packet.second));
    }

    std::sort(std::begin(sorted), std::end(sorted), [](const auto& item1, const auto& item2)
    {
        return Compare(&item1, &item2) == Cmp::Lt;
    });

    size_t idx = 1;
    size_t result = 1;
    for (const auto& packet : sorted)
    {
        //std::cout << packet << std::endl;
        if (isDivider(&packet, dividerValue1) || isDivider(&packet, dividerValue2))
        {
            result *= idx;
        }
        ++idx;
    }

    return result;
}

List *getList(std::unique_ptr<Item>& item)
{
    return dynamic_cast<List *>(item.get());
}

Value *getValue(std::unique_ptr<Item>& item)
{
    return dynamic_cast<Value *>(item.get());
}

TEST_CASE("2022-day-13-parse")
{
    std::stringstream ss;

    {
        ss << "[1,1,3,1,1]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 5);
    }

    {
        ss.clear();
        ss << "[[1],[2,3,4]]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 2);

        const auto list1 = getList(list.data[0]);
        REQUIRE(list1->data.size() == 1);

        const auto value1 = getValue(list1->data[0]);
        REQUIRE(value1->value == 1);

        const auto list2 = getList(list.data[1]);
        REQUIRE(list2->data.size() == 3);

        const auto value2 = getValue(list2->data[0]);
        REQUIRE(value2->value == 2);
    }

    {
        ss.clear();
        ss << "[[1],4]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 2);

        const auto list1 = getList(list.data[0]);
        REQUIRE(list1->data.size() == 1);

        const auto value1 = getValue(list1->data[0]);
        REQUIRE(value1->value == 1);

        const auto value2 = getValue(list.data[1]);
        REQUIRE(value2->value == 4);
    }

    {
        ss.clear();
        ss << "[[8,7,6]]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 1);

        const auto list1 = getList(list.data[0]);
        REQUIRE(list1->data.size() == 3);

        REQUIRE(getValue(list1->data[0])->value == 8);
        REQUIRE(getValue(list1->data[1])->value == 7);
        REQUIRE(getValue(list1->data[2])->value == 6);
    }

    {
        ss.clear();
        ss << "[[4,4],4,4]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 3);

        const auto list1 = getList(list.data[0]);
        REQUIRE(list1->data.size() == 2);

        REQUIRE(getValue(list1->data[0])->value == 4);
        REQUIRE(getValue(list1->data[1])->value == 4);

        REQUIRE(getValue(list.data[1])->value == 4);
        REQUIRE(getValue(list.data[2])->value == 4);
    }

    {
        ss.clear();
        ss << "[[[]]]";
        List list;
        ss >> list;
        REQUIRE(list.data.size() == 1);

        const auto list1 = getList(list.data[0]);
        REQUIRE(list1->data.size() == 1);

        const auto list2 = getList(list1->data[0]);
        REQUIRE(list2->data.size() == 0);
    }
}

TEST_CASE("2022-day-13-solve")
{
    std::stringstream ss;

    {
        ss << "[[],[9]]" << std::endl
           << "[[],[[4,[5]]]]" << std::endl;

        const auto data = parse(ss);
        const auto res = Compare(&data[0].first, &data[0].second);
        REQUIRE(res == Cmp::Gt);
    }

    {
        ss.clear();
        ss << "[[9]]" << std::endl
           << "[[[4,[5]]]]" << std::endl;

        const auto data = parse(ss);
        const auto res = Compare(&data[0].first, &data[0].second);
        REQUIRE(res == Cmp::Gt);
    }
}
}

TEST_CASE("2022-day-13")
{
    auto input = Utility::getTestData("13/test-input.txt");
    auto data = parse(input);

    REQUIRE(data.size() == 8);
    REQUIRE(solvePart1(data) == 13);

    REQUIRE(solvePart2(std::move(data)) == 140);
}

void solution()
{
    auto data = parse(std::cin);
    std::cout << solvePart1(data) << std::endl;
    std::cout << solvePart2(std::move(data)) << std::endl;
}
}
