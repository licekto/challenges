package solution04

import (
	"aoc2021/utility"
    "testing"
)

func TestSolution04Sample(t *testing.T) {
	samplePath := utility.TestDataPath + "/04/sample.txt"

    sampleFile := utility.OpenFile(samplePath)
    defer sampleFile.Close()
	game := parse(sampleFile)

    utility.Assert(t, "solution04, parse", len(game.Numbers), 27)
    utility.Assert(t, "solution04, parse", game.Numbers[0], 7)
    utility.Assert(t, "solution04, parse", game.Numbers[26], 1)
    utility.Assert(t, "solution04, parse", len(game.Boards), 3)
    utility.Assert(t, "solution04, parse", game.Boards[0][0][0].Value, 22)
    utility.Assert(t, "solution04, parse", game.Boards[2][4][4].Value, 7)

    part1, part2 := execGame(game)
    utility.Assert(t, "solution04, part1", part1, 4512)
    utility.Assert(t, "solution04, part2", part2, 1924)
}

func TestSolution04(t *testing.T) {
    utility.TestRealData(t, Solve, "04", 65325, 4624)
}
