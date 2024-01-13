#include <catch2/catch_all.hpp>

#include "01.hpp"
#include "02.hpp"
#include "03.hpp"
#include "04.hpp"
#include "05.hpp"
#include "05.hpp"
#include "06.hpp"
#include "07.hpp"
#include "08.hpp"
#include "09.hpp"
#include "10.hpp"
#include "11.hpp"
#include "12.hpp"
#include "13.hpp"
#include "14.hpp"
#include "15.hpp"
#include "16.hpp"
#include "17.hpp"
#include "18.hpp"
#include "19.hpp"
#include "20.hpp"
#include "21.hpp"
#include "22.hpp"
#include "23.hpp"
#include "24.hpp"
#include "25.hpp"

#include <functional>
#include <iostream>

const std::pair<std::string, std::function<void()>> daySolutions[] =
{
    {"01", aoc01::solution},
    {"02", aoc02::solution},
    {"03", aoc03::solution},
    {"04", aoc04::solution},
    {"05", aoc05::solution},
    {"06", aoc06::solution},
    {"07", aoc07::solution},
    {"08", aoc08::solution},
    {"09", aoc09::solution},
    {"10", aoc10::solution},
    {"11", aoc11::solution},
    {"12", aoc12::solution},
    {"13", aoc13::solution},
    {"14", aoc14::solution},
    {"15", aoc15::solution},
    {"16", aoc16::solution},
    {"17", aoc17::solution},
    {"18", aoc18::solution},
    {"19", aoc19::solution},
    {"20", aoc20::solution},
    {"21", aoc21::solution},
    {"22", aoc22::solution},
    {"23", aoc23::solution},
    {"24", aoc24::solution},
    {"25", aoc25::solution}
};

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "No action" << std::endl;
        return 1;
    }

    const std::string arg1(argv[1]);
    if (arg1 == "--test")
    {
        Catch::Session().run(argc - 1, argv);
        return 0;
    }

    if (arg1 != "--day")
    {
        std::cerr << "Unknown assignment" << std::endl;
        return 1;
    }

    if (argc != 3)
    {
        std::cerr << "Missing paramater for the " << arg1 << " argument" << std::endl;
        return 1;
    }

    const std::string arg2(argv[2]);

    bool called = false;
    for (const auto& [funcName, func] : daySolutions)
    {
        if (funcName == arg2)
        {
            func();
            called = true;
        }
    }

    if (!called)
    {
        std::cerr << "Unknown assignment" << std::endl;
        return 1;
    }

    return 0;
}

