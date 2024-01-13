package solution06


import (
    "aoc2021/utility"
	"os"
)

func simulate(numbers []int64, days int64) int64 {
    stages := make(map[int64]int64)

    for _, n := range numbers {
        stages[n]++
    }

    for d := int64(0); d < days; d++ {
        tmpStages := make(map[int64]int64)
        tmpStages[6] = stages[0]
        tmpStages[8] = stages[0]

        for i := int64(1); i <= 8; i++ {
            tmpStages[i - 1] += stages[i]
        }

        stages = tmpStages
    }

    var sum int64
    for _, v := range stages {
        sum += v
    }

    return sum
}

func Solve(file *os.File) (int64, int64) {
    numbers := utility.ParseCSV(file)
    return simulate(numbers, 80), simulate(numbers, 256)
}

