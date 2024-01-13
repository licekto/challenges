package solution01

import (
	"aoc2021/utility"
	"os"
)

func parse(lines []string) []int64 {
	var data []int64
    for _, str := range lines {
        data = append(data, utility.Stoi(str))
    }
	return data
}

func solvePart1(data []int64) int64 {
	prev := data[0]
	var count int64

	for _, v := range data[1:] {
		if prev < v {
			count++
		}
		prev = v
	}

	return count
}

func solvePart2(data []int64) int64 {
	sum3 := data[0] + data[1] + data[2]
    prev := sum3
	var count int64

	for i := 3; i < len(data); i++ {
        sum3 -= data[i - 3]
        sum3 += data[i]
		if prev < sum3 {
			count++
		}
		prev = sum3
	}

	return count
}

func Solve(file *os.File) (int64, int64) {
    data := parse(utility.GetLines(file))
	return solvePart1(data), solvePart2(data)
}

