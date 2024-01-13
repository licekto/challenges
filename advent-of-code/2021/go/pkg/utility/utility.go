package utility

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
    "strings"
    "testing"
    "sort"
)

func Stoi(str string) int64 {
	val, err := strconv.ParseInt(str, 10, 64)
	if err != nil {
		fmt.Println("Error when converting ", str, " to string")
		panic(err)
	}
	return val
}

func OpenFile(path string) *os.File {
    file, err := os.Open(path)

	if err != nil {
		fmt.Println("Can't open the ", path, " file")
		panic(err)
	}

    return file
}

func GetLines(file *os.File) []string {
	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)

	var lines []string

	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	return lines
}

func GetLinesFromFile(path string) []string {
	file, err := os.Open(path)
	defer file.Close()

	if err != nil {
		fmt.Println("Can't open the ", path, " file")
		panic(err)
	}

	return GetLines(file)
}

const TestDataPath = "../../data"

func Assert[T comparable](t *testing.T, descr string, value, expected T) {
   if value != expected {
        t.Fatalf(fmt.Sprintf("%s: value=%v != expected=%v", descr, value, expected))
   } 
}

type Solver func(*os.File) (int64, int64)

func TestRealData(t *testing.T, solver Solver, day string, part1Exp, part2Exp int64) {
	path := TestDataPath + "/" + day + "/input.txt"

    file := OpenFile(path)
    defer file.Close()

    part1, part2 := solver(file)

    Assert(t, "solution" + day + ", part1", part1, part1Exp)
    Assert(t, "solution" + day + ", part2", part2, part2Exp)
}

func Max(x, y int64) int64 {
    if x < y {
        return y
    }
    return x
}

func Min(x, y int64) int64 {
    if x < y {
        return x
    }
    return y
}

func Abs(x int64) int64 {
    if x < 0 {
        return -x
    }
    return x
}

func Swap[T any](val1, val2 *T) {
    tmp := *val1
    *val1 = *val2
    *val2 = tmp
}

func ParseCSV(file *os.File) []int64 {
    scanner := bufio.NewScanner(file)
    scanner.Split(bufio.ScanLines)
    scanner.Scan()
    numStrings := strings.Split(scanner.Text(), ",")

    var numbers []int64
    for _, nStr := range numStrings {
        numbers = append(numbers, Stoi(nStr))
    }
    return numbers
}

type Range64 []int64

func (a Range64) Len() int {
    return len(a)
}

func (a Range64) Swap(i, j int) {
    a[i], a[j] = a[j], a[i]
}

func (a Range64) Less(i, j int) bool {
    return a[i] < a[j]
}

func Sort(a Range64) {
    sort.Sort(a)
}

func Median(numbers []int64) int64 {
    if len(numbers) == 0 {
        return 0
    }

    var numbersCopy []int64
    copy(numbersCopy, numbers)

    Sort(numbersCopy)
    if len(numbersCopy) % 2 == 0 {
        return (numbers[(len(numbers) / 2) - 1] + numbers[len(numbers) / 2]) / 2
    }
    return numbers[len(numbers) / 2]
}

func Sum(numbers []int64) int64 {
    var sum int64
    for _, n := range numbers {
        sum += n
    }
    return sum
}

func Mean(numbers []int64) float64 {
    return float64(Sum(numbers)) / float64(len(numbers))
}

func TriangleNumber(n int64) int64 {
    return n * (n + 1) / 2
}

