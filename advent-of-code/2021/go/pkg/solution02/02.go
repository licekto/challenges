package solution02


import (
    "aoc2021/utility"
    "bufio"
//    "fmt"
	"os"
)

type Move struct {
    Dir byte
    Count int64
}

func parse(file *os.File) []Move {
    scanner := bufio.NewScanner(file)
    scanner.Split(bufio.ScanWords)

    moves := []Move{}

    for scanner.Scan() {
        dir := scanner.Text()[0]
        scanner.Scan()
        count := utility.Stoi(scanner.Text())
        moves = append(moves, Move{dir, count})
    }

    return moves
}

func solvePart1(moves []Move) int64 {
    var horizontal, depth int64

    for _, m := range moves {
        switch m.Dir {
            case 'f': horizontal += m.Count
            case 'd': depth += m.Count
            case 'u': depth -= m.Count
        }
    }

    return horizontal * depth
}

func solvePart2(moves []Move) int64 {
    var aim, horizontal, depth int64

    for _, m := range moves {
        switch m.Dir {
            case 'f':
                horizontal += m.Count
                depth += aim * m.Count
            case 'd': aim += m.Count
            case 'u': aim -= m.Count
        }
    }

    return horizontal * depth
}

func Solve(file *os.File) (int64, int64) {
    data := parse(file)
    return solvePart1(data), solvePart2(data)
}

