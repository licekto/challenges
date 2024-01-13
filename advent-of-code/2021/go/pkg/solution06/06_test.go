package solution06

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution06Sample(t *testing.T) {
	samplePath := utility.TestDataPath + "/06/sample.txt"

    sampleFile := utility.OpenFile(samplePath)
    defer sampleFile.Close()
	numbers := utility.ParseCSV(sampleFile)

    utility.Assert(t, "solution06, parse", len(numbers), 5)
    utility.Assert(t, "solution06, parse", numbers[0], 3)
    utility.Assert(t, "solution06, parse", numbers[4], 2)
    
    n1 := make([]int64, len(numbers))
    copy(n1, numbers)
    utility.Assert(t, "solution06, simulate", simulate(n1, 18), 26)

    n2 := make([]int64, len(numbers))
    copy(n2, numbers)
    utility.Assert(t, "solution06, simulate", simulate(n2, 80), 5934)
    
    n3 := make([]int64, len(numbers))
    copy(n3, numbers)
    utility.Assert(t, "solution06, simulate", simulate(n3, 256), 26984457539)
}

func TestSolution06(t *testing.T) {
    utility.TestRealData(t, Solve, "06", 375482, 1689540415957)
}
