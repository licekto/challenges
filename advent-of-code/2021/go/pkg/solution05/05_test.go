package solution05

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution05Sample(t *testing.T) {
	samplePath := utility.TestDataPath + "/05/sample.txt"

    sampleFile := utility.OpenFile(samplePath)
    defer sampleFile.Close()
	lines := parse(sampleFile)

    utility.Assert(t, "solution05, parse", len(lines), 10)
    utility.Assert(t, "solution05, parse", lines[0].P1.X, 0)
    utility.Assert(t, "solution05, parse", lines[0].P1.Y, 9)
    utility.Assert(t, "solution05, parse", lines[0].P2.X, 5)
    utility.Assert(t, "solution05, parse", lines[0].P2.Y, 9)
    utility.Assert(t, "solution05, parse", lines[9].P1.X, 5)
    utility.Assert(t, "solution05, parse", lines[9].P1.Y, 5)
    utility.Assert(t, "solution05, parse", lines[9].P2.X, 8)
    utility.Assert(t, "solution05, parse", lines[9].P2.Y, 2)

    part1, part2 := addLinesAndCount(lines)
    utility.Assert(t, "solution05, part1", part1, 5)
    utility.Assert(t, "solution05, part1", part2, 12)
}

func TestSolution05(t *testing.T) {
    utility.TestRealData(t, Solve, "05", 5147, 16925)
}
