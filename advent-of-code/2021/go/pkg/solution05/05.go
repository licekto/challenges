package solution05


import (
    "aoc2021/utility"
    "bufio"
    "fmt"
	"os"
)

type Coord struct {
    X, Y int64
}

type Line struct {
    P1, P2 Coord
}

type Lines []Line

func parse(file *os.File) Lines {
    scanner := bufio.NewScanner(file)
    scanner.Split(bufio.ScanLines)

    var lines Lines
    for scanner.Scan() {
        var line Line
        fmt.Sscanf(scanner.Text(), "%d,%d -> %d,%d", &line.P1.X, &line.P1.Y, &line.P2.X, &line.P2.Y)
        lines = append(lines, line)
    }

    return lines
}

type Coords map[Coord]int64

func (coords *Coords) addHorizontalLine(line Line) {
    if line.P1.X == line.P2.X {
        for y := utility.Min(line.P1.Y, line.P2.Y); y <= utility.Max(line.P1.Y, line.P2.Y); y++ {
            (*coords)[Coord{line.P1.X, y}] += 1
        }
    }
}

func (coords *Coords) addVerticalLine(line Line) {
    if line.P1.Y == line.P2.Y {
        for x := utility.Min(line.P1.X, line.P2.X); x <= utility.Max(line.P1.X, line.P2.X); x++ {
            (*coords)[Coord{x, line.P1.Y}] += 1
        }
    }
}

func (coords *Coords) addDiagonalLine(line Line) {
    if line.P1.X == line.P2.X || line.P1.Y == line.P2.Y {
        return
    }
    if line.P1.X > line.P2.X {
        utility.Swap[Coord](&line.P1, &line.P2)
    }
    if line.P1.Y < line.P2.Y {
        for x, y := line.P1.X, line.P1.Y; x <= line.P2.X; x, y = x + 1, y + 1 {
            (*coords)[Coord{x, y}] += 1
        }
    } else if line.P1.Y > line.P2.Y {
        for x, y := line.P1.X, line.P1.Y; x <= line.P2.X; x, y = x + 1, y - 1 {
            (*coords)[Coord{x, y}] += 1
        }
    }
}

func (coords *Coords) countOverlaps() int64 {
    var overlapCount int64
    for _, v := range *coords {
        if v >= 2 {
            overlapCount++
        }
    }
    return overlapCount
}

func addLinesAndCount(lines Lines) (int64, int64) {
    coords := make(Coords)

    for _, line := range lines {
        coords.addHorizontalLine(line)
        coords.addVerticalLine(line)
    }
    count1 := coords.countOverlaps()
    
    for _, line := range lines {
        coords.addDiagonalLine(line)
    }
    count2 := coords.countOverlaps()

    return count1, count2
}

func Solve(file *os.File) (int64, int64) {
    lines := parse(file)
    return addLinesAndCount(lines)
}

