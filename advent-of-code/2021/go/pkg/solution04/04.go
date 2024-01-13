package solution04


import (
    "aoc2021/utility"
    "bufio"
    "fmt"
	"os"
    "strings"
)

type Item struct {
    Value int64
    Drawn bool
}

const Size = 5

type Board [Size][Size]Item

func (board *Board) isWinner() bool {
    for i := 0; i < Size; i++ {
        rowComplete := true
        colComplete := true
        for j := 0; j < Size; j++ {
            if !board[i][j].Drawn {
                rowComplete = false
            }
            if !board[j][i].Drawn {
                colComplete = false;
            }
        }
        if rowComplete || colComplete {
            return true
        }
    }
    return false
}

func (board *Board) print() {
    for i := 0; i < Size; i++ {
        for j := 0; j < Size; j++ {
            fmt.Print(board[i][j].Value)
            if board[i][j].Drawn {
                fmt.Print("*")
            }
            fmt.Print(" ")
        }
        fmt.Println()
    }
}

func (board *Board) execDraw(draw int64) {
    for i := 0; i < Size; i++ {
        for j := 0; j < Size; j++ {
            if board[i][j].Value == draw {
                board[i][j].Drawn = true
            }
        }
    }
}

func (board *Board) sumNonDrawn() (sum int64) {
    for i := 0; i < Size; i++ {
        for j := 0; j < Size; j++ {
            if !board[i][j].Drawn {
                sum += board[i][j].Value
            }
        }
    }
    return
}

type Game struct {
    Numbers []int64
    Boards []Board
    FinishedBoards []bool
}

func (game *Game) execDraw(draw int64) (winnerFound bool, winnerBoardIdx int) {
    for b := 0; b < len(game.Boards); b++ {
        if game.FinishedBoards[b] {
            continue
        }
        game.Boards[b].execDraw(draw)
        if game.Boards[b].isWinner() {
            game.FinishedBoards[b] = true
            winnerFound = true
            winnerBoardIdx = b
        }
    }
    if winnerFound {
        return winnerFound, winnerBoardIdx
    }
    return false, -1
}

func parse(file *os.File) Game {
    scanner := bufio.NewScanner(file)
    scanner.Split(bufio.ScanWords)

    var game Game

    scanner.Scan()
    line := strings.Split(scanner.Text(), ",")
    for _, valStr := range line {
        game.Numbers = append(game.Numbers, utility.Stoi(valStr))
    }

    for {
        end := false
        var board Board
        for i := 0; i < Size; i++ {
            for j := 0; j < Size; j++ {
                scanner.Scan()
                valStr := scanner.Text()
                if len(valStr) == 0 {
                    end = true
                    break
                }
                board[i][j].Value = utility.Stoi(valStr)
            }
            if end {
                break
            }
        }
        if end {
            break
        }
        game.Boards = append(game.Boards, board)
        game.FinishedBoards = append(game.FinishedBoards, false)
    }

    return game
}

func execGame(game Game) (first int64, last int64) {
    var firstWinnerFound bool
    for _, draw := range game.Numbers {
        winnerFound, winnerIdx := game.execDraw(draw)
        if winnerFound {
            if !firstWinnerFound {
                first = draw * game.Boards[winnerIdx].sumNonDrawn()
                firstWinnerFound = true
            }

            last = game.Boards[winnerIdx].sumNonDrawn() * draw
        }
    }
    return
}

func Solve(file *os.File) (int64, int64) {
    return execGame(parse(file))
}

