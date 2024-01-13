package main

import (
	"aoc2021/solution01"
	"aoc2021/solution02"
	"aoc2021/solution03"
	"aoc2021/solution04"
	"aoc2021/solution05"
	"aoc2021/solution06"
	"aoc2021/solution07"
	"aoc2021/solution08"
	"aoc2021/utility"
	"fmt"
	"os"
)

type Solver interface {
	Solve() (int64, int64)
}

func main() {
    days := []func(file *os.File)(int64, int64){solution01.Solve,
                                                solution02.Solve,
                                                solution03.Solve,
                                                solution04.Solve,
                                                solution05.Solve,
                                                solution06.Solve,
                                                solution07.Solve,
                                                solution08.Solve}

	args := os.Args[1:]

	if args[0] == "--day" {
        day := utility.Stoi(args[1])
        if day < 1 || day > 25 {
            panic("Day is not in the <1,25> range")
        }
		part1, part2 := days[day - 1](os.Stdin)
		fmt.Println(part1)
		fmt.Println(part2)
	}

}
