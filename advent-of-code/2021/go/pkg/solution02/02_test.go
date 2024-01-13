package solution02

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution02Sample(t *testing.T) {
	path := utility.TestDataPath + "/02/sample.txt"

    file := utility.OpenFile(path)
    defer file.Close()
	data := parse(file)

    utility.Assert(t, "solution02, parse", data[0], Move{'f', 5})
    utility.Assert(t, "solution02, parse", data[1], Move{'d', 5})
    utility.Assert(t, "solution02, parse", data[2], Move{'f', 8})
    utility.Assert(t, "solution02, parse", data[3], Move{'u', 3})
    utility.Assert(t, "solution02, parse", data[4], Move{'d', 8})
    utility.Assert(t, "solution02, parse", data[5], Move{'f', 2})

    utility.Assert(t, "solution02, part1", solvePart1(data), 150)
    utility.Assert(t, "solution02, part2", solvePart2(data), 900)
}

func TestSolution02(t *testing.T) {
    utility.TestRealData(t, Solve, "02", 2039912, 1942068080)
}
