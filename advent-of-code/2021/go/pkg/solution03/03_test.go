package solution03

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution03Sample(t *testing.T) {
	samplePath := utility.TestDataPath + "/03/sample.txt"

    sampleFile := utility.OpenFile(samplePath)
    defer sampleFile.Close()
	sampleData := parse(sampleFile)

    utility.Assert(t, "solution03, parse", len(sampleData), 12)
    utility.Assert(t, "solution03, parse", sampleData[0], "00100")
    utility.Assert(t, "solution03, parse", sampleData[1], "11110")
    utility.Assert(t, "solution03, parse", sampleData[11], "01010")

    utility.Assert(t, "solution03, part1", solvePart1(sampleData), 198)
    utility.Assert(t, "solution03, part2", solvePart2(sampleData), 230)
}

func TestSolution03(t *testing.T) {
    utility.TestRealData(t, Solve, "03", 3969000, 4267809)
}
