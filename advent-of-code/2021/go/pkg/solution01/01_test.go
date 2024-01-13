package solution01

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution01Sample(t *testing.T) {
	data := parse(utility.GetLinesFromFile(utility.TestDataPath + "/01/sample.txt"))
    utility.Assert(t, "solution01, part1", solvePart1(data), 7)
    utility.Assert(t, "solution01, part2", solvePart2(data), 5)
}

func TestSolution01(t *testing.T) {
    utility.TestRealData(t, Solve, "01", 1162, 1190)
}

