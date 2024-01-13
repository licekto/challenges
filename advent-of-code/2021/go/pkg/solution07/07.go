package solution07

import (
    "aoc2021/utility"
//    "fmt"
	"os"
)

func solvePart1(numbers []int64) int64 {
    utility.Sort(numbers)

    middle := numbers[len(numbers) / 2]

    var sum int64
    for _, n := range numbers {
        sum += utility.Abs(n - middle)
    }

    return sum
}

func solvePart2(numbers []int64) int64 {
    target := utility.Mean(numbers)

    var summedCost = func (target float64) int64 {
        var sum int64
        for _, n := range numbers {
            sum += utility.TriangleNumber(utility.Abs(n - int64(target)))
        }
        return sum
    }

    res1 := summedCost(float64(target) - 0.5)
    res2 := summedCost(float64(target))
    res3 := summedCost(float64(target) + 0.5)

    return utility.Min(res1, utility.Min(res2, res3))
}

func Solve(file *os.File) (int64, int64) {
    numbers := utility.ParseCSV(file)
    return solvePart1(numbers), solvePart2(numbers)
}

