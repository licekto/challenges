#include "07.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <variant>
#include <vector>

namespace aoc07
{
namespace
{

namespace Commands
{
struct File
{
    std::string name;
    size_t size;
};

struct Dir
{
    std::string name;
};

struct Cd
{
    enum class ParamType { Current, Up, Root };
    ParamType type;
    std::string param;
};

struct Ls
{};

using InputType = std::variant<File, Dir, Cd, Ls>;
}
using CommandLine = std::vector<Commands::InputType>;

static constexpr std::string_view Prompt = "$";
static constexpr std::string_view DirIdent = "dir";
static constexpr std::string_view CdIdent = "cd";
static constexpr std::string_view LsIdent = "ls";
static constexpr std::string_view DirUp = "..";
static constexpr std::string_view DirRoot = "/";

CommandLine parse(std::istream& is)
{
    CommandLine data;

    std::string token;
    Commands::File f;
    Commands::Dir d;
    Commands::Cd cd;

    while (1)
    {
        is >> token;
        if (is.eof())
        {
            break;
        }

        if (token == Prompt)
        {
            is >> token;
            if (token == CdIdent)
            {
                is >> token;
                if (token == DirUp)
                {
                    cd.type = Commands::Cd::ParamType::Up;
                }
                else if (token == DirRoot)
                {
                    cd.type = Commands::Cd::ParamType::Root;
                }
                else
                {
                    cd.type = Commands::Cd::ParamType::Current;
                    cd.param = token;
                }
                data.push_back(cd);
            }
            else if (token == LsIdent)
            {
                data.push_back(Commands::Ls{});
            }
            else
            {
                throw std::runtime_error("Unknown command: '" + token + "'");
            }
        }
        else if (token == DirIdent)
        {
            is >> token;
            data.push_back(Commands::Dir{token});
        }
        else
        {
            const size_t fileSize = std::stoul(token);
            is >> token;
            data.push_back(Commands::File{token, fileSize});
        }
    }

    return data;
}

class FileSystem
{
public:
    FileSystem();
    void Cd(const Commands::Cd& command);
    void AddDir(const std::string& name);
    void AddFile(const std::string& name, const size_t size);
    void CalculateSizes();
    void Print() const;

    size_t SolvePart1() const;
    size_t SolvePart2() const;

private:
    struct File
    {
        std::string name;
        size_t size;
    };

    struct Dir
    {
        Dir(const std::string& name);
        std::string name;
        size_t size = 0;

        using Elem = std::variant<File, std::unique_ptr<Dir>>;

        Dir *parentDir = nullptr;
        std::vector<Elem*> elementsOrder {};
        std::unordered_map<std::string, Elem> elements {};
    };

    static void printRec(const Dir * const dir, const size_t indentSize);
    static void indent(const size_t indentSize);

    size_t calculateDirSizeRec(Dir *dir);

    size_t solvePart1Rec(const Dir *dir) const;
    void solvePart2Rec(const Dir *dir, const size_t toBeFreedMin, size_t& currentMin) const;

    Dir rootDir;
    Dir *currentDir = nullptr;
};

FileSystem::Dir::Dir(const std::string& name)
    : name(name)
{}

FileSystem::FileSystem()
    : rootDir{std::string(DirRoot)}
{}

void FileSystem::Cd(const Commands::Cd& command)
{
    if (command.type == Commands::Cd::ParamType::Current)
    {
        const auto it = currentDir->elements.find(command.param);
        if (it == std::end(currentDir->elements))
        {
            throw std::runtime_error("The '" + command.param + "' directory is not present inside the '" + currentDir->name + "' directory.");
        }
        currentDir = std::get<std::unique_ptr<Dir>>(it->second).get();
    }
    else if (command.type == Commands::Cd::ParamType::Up)
    {
        currentDir = currentDir->parentDir;
    }
    else if (command.type == Commands::Cd::ParamType::Root)
    {
        currentDir = &rootDir;
    }
}

void FileSystem::AddDir(const std::string& name)
{
    auto newDir = std::make_unique<Dir>(name);
    newDir->parentDir = currentDir;
    currentDir->elements[name] = std::move(newDir);
    currentDir->elementsOrder.push_back(&currentDir->elements[name]);
}

void FileSystem::AddFile(const std::string& name, const size_t size)
{
    currentDir->elements[name] = File{name, size};
    currentDir->elementsOrder.push_back(&currentDir->elements[name]);
}

void FileSystem::CalculateSizes()
{
    calculateDirSizeRec(&rootDir);
}

void FileSystem::Print() const
{
    printRec(&rootDir, 0);
}

void FileSystem::printRec(const Dir * const dir, const size_t indentSize)
{
    indent(indentSize);
    std::cout << "- " << dir->name << " (dir)" << std::endl;

    for (const auto& elem : dir->elementsOrder)
    {
        if (std::holds_alternative<File>(*elem))
        {
            indent(indentSize + 2);
            const auto& file = std::get<File>(*elem);
            std::cout << "- " << file.name << " (file, size=" << file.size << ")" << std::endl;
        }
        else
        {
            printRec(std::get<std::unique_ptr<Dir>>(*elem).get(), indentSize + 2);
        }
    }
}

void FileSystem::indent(const size_t indentSize)
{
    for (size_t i = 0; i < indentSize; ++i)
    {
        std::cout << " ";
    }
}

size_t FileSystem::calculateDirSizeRec(Dir *dir)
{
    for (const auto& [elemName, elem] : dir->elements)
    {
        if (std::holds_alternative<File>(elem))
        {
            dir->size += std::get<File>(elem).size;
        }
        else
        {
            dir->size += calculateDirSizeRec(std::get<std::unique_ptr<Dir>>(elem).get());
        }
    }
    return dir->size;
}

size_t FileSystem::SolvePart1() const
{
    return solvePart1Rec(&rootDir);
}

size_t FileSystem::solvePart1Rec(const Dir *dir) const
{
    static constexpr size_t maxDirSize = 100000;

    size_t sum = 0;
    if (dir->size <= maxDirSize)
    {
        sum += dir->size;
    }

    for (const auto& elem : dir->elementsOrder)
    {
        if (std::holds_alternative<std::unique_ptr<Dir>>(*elem))
        {
            sum += solvePart1Rec(std::get<std::unique_ptr<Dir>>(*elem).get());
        }
    }

    return sum;
}

size_t FileSystem::SolvePart2() const
{
    static constexpr size_t diskSize = 70000000;
    static constexpr size_t required = 30000000;
    const size_t used = rootDir.size;

    const size_t free = diskSize - used;
    const size_t toBeFreedMin = required - free;

    size_t currentMin = -1;

    solvePart2Rec(&rootDir, toBeFreedMin, currentMin);

    return currentMin;
}

void FileSystem::solvePart2Rec(const Dir *dir, const size_t toBeFreedMin, size_t& currentMin) const
{
    if (dir->size >= toBeFreedMin && dir->size < currentMin)
    {
        currentMin = dir->size;
    }

    for (const auto& elem : dir->elementsOrder)
    {
        if (std::holds_alternative<std::unique_ptr<Dir>>(*elem))
        {
            solvePart2Rec(std::get<std::unique_ptr<Dir>>(*elem).get(), toBeFreedMin, currentMin);
        }
    }
}

void Execute(FileSystem& fileSystem, const CommandLine& commandLine)
{
    for (const auto& line : commandLine)
    {
        if (std::holds_alternative<Commands::File>(line))
        {
            const auto& file = std::get<Commands::File>(line);
            fileSystem.AddFile(file.name, file.size);
        }
        else if (std::holds_alternative<Commands::Dir>(line))
        {
            const auto& dir = std::get<Commands::Dir>(line);
            fileSystem.AddDir(dir.name);
        }
        else if (std::holds_alternative<Commands::Cd>(line))
        {
            const auto& cd = std::get<Commands::Cd>(line);
            fileSystem.Cd(cd);
        }
    }

    fileSystem.CalculateSizes();
}

}

namespace tests
{
void testFile(const Commands::InputType& input, const std::string& name, const size_t size)
{
    REQUIRE(std::get<Commands::File>(input).name == name);
    REQUIRE(std::get<Commands::File>(input).size == size);
}

void testDir(const Commands::InputType& input, const std::string& name)
{
    REQUIRE(std::get<Commands::Dir>(input).name == name);
}

void testCd(const Commands::InputType& input, const Commands::Cd::ParamType type, const std::string& param = "")
{
    REQUIRE(std::get<Commands::Cd>(input).type == type);
    if (type == Commands::Cd::ParamType::Current)
    {
        REQUIRE(std::get<Commands::Cd>(input).param == param);
    }
}

void testLs(const Commands::InputType& input)
{
    REQUIRE(std::holds_alternative<Commands::Ls>(input));
}
}

TEST_CASE("2022-day-07")
{
    std::stringstream ss;
    ss << R"($ cd /
          $ ls
          dir a
          14848514 b.txt
          8504156 c.dat
          dir d
          $ cd a
          $ ls
          dir e
          29116 f
          2557 g
          62596 h.lst
          $ cd e
          $ ls
          584 i
          $ cd ..
          $ cd ..
          $ cd d
          $ ls
          4060174 j
          8033020 d.log
          5626152 d.ext
          7214296 k)";

    const auto commandLine = parse(ss);

    REQUIRE(commandLine.size() == 23);

    tests::testCd(commandLine[0], Commands::Cd::ParamType::Root);
    tests::testLs(commandLine[1]);
    tests::testDir(commandLine[2], "a");
    tests::testFile(commandLine[3], "b.txt", 14848514);
    tests::testFile(commandLine[4], "c.dat", 8504156);
    tests::testDir(commandLine[5], "d");
    tests::testCd(commandLine[6], Commands::Cd::ParamType::Current, "a");
    tests::testLs(commandLine[7]);
    tests::testDir(commandLine[8], "e");
    tests::testFile(commandLine[9], "f", 29116);
    tests::testFile(commandLine[10], "g", 2557);
    tests::testFile(commandLine[11], "h.lst", 62596);
    tests::testCd(commandLine[12], Commands::Cd::ParamType::Current, "e");
    tests::testLs(commandLine[13]);
    tests::testFile(commandLine[14], "i", 584);
    tests::testCd(commandLine[15], Commands::Cd::ParamType::Up);
    tests::testCd(commandLine[16], Commands::Cd::ParamType::Up);
    tests::testCd(commandLine[17], Commands::Cd::ParamType::Current, "d");
    tests::testLs(commandLine[18]);
    tests::testFile(commandLine[19], "j", 4060174);
    tests::testFile(commandLine[20], "d.log", 8033020);
    tests::testFile(commandLine[21], "d.ext", 5626152);
    tests::testFile(commandLine[22], "k", 7214296);

    FileSystem fileSystem;

    Execute(fileSystem, commandLine);

    //fileSystem.Print();

    REQUIRE(fileSystem.SolvePart1() == 95437);
    REQUIRE(fileSystem.SolvePart2() == 24933642);
}

void solution()
{
    const auto commandLine = parse(std::cin);

    FileSystem fileSystem;
    Execute(fileSystem, commandLine);

    std::cout << fileSystem.SolvePart1() << std::endl;
    std::cout << fileSystem.SolvePart2() << std::endl;
}
}
