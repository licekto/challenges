package solution07

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution07Sample(t *testing.T) {

	samplePath := utility.TestDataPath + "/07/sample.txt"

    sampleFile := utility.OpenFile(samplePath)
    defer sampleFile.Close()
	numbers := utility.ParseCSV(sampleFile)

    utility.Assert(t, "solution07, parse", len(numbers), 10)
    utility.Assert(t, "solution07, solvePart1", solvePart1(numbers), 37)
    utility.Assert(t, "solution07, solvePart2", solvePart2(numbers), 168)
}

func TestSolution07(t *testing.T) {
    utility.TestRealData(t, Solve, "07", 337833, 96678050)
}
