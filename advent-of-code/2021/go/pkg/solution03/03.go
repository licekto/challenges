package solution03


import (
    "bufio"
	"os"
    "strconv"
)

func parse(file *os.File) []string {
    scanner := bufio.NewScanner(file)
    scanner.Split(bufio.ScanWords)

    numbers := []string{}

    for scanner.Scan() {
        numbers = append(numbers, scanner.Text())
    }

    return numbers
}

type Item struct {
    Value string
    Present bool
}

func markNotPresent(items []Item, char byte, idx int) {
    for i := 0; i < len(items); i++ {
        if items[i].Value[idx] == char {
            items[i].Present = false
        }
    }
}

type criterion func(int64, int64) byte

func calculateCrit(itemsOrig []Item, crit criterion) int64 {

    items := make([]Item, len(itemsOrig))
    copy(items, itemsOrig)

    for i := 0; i < len(items[0].Value); i++ {
        var zeroCount, oneCount, itemsPresent int64

        for _, item := range items {
            if !item.Present {
                continue
            }
            itemsPresent++
            if item.Value[i] == '0' {
                zeroCount++;
            } else if item.Value[i] == '1' {
                oneCount++
            }
        }

        if itemsPresent == 1 {
            break
        }

        markNotPresent(items, crit(zeroCount, oneCount), i)
    }

    for _, item := range items {
        if item.Present {
            val, _ := strconv.ParseInt(item.Value, 2, 64)
            return val
        }
    }

    return 0
}

func calculateGammaEps(numbers []string) (int64, int64) {
    var gamma, neg int64

    for i := 0; i < len(numbers[0]); i++ {
        neg = neg << 1
        neg = neg | 1
        var zeroCount, oneCount int64

        for _, n := range numbers {
            if n[i] == '0' {
                zeroCount++;
            } else if n[i] == '1' {
                oneCount++
            }
        }

        gamma = gamma << 1
        if zeroCount < oneCount {
            gamma = gamma | 1
        }
    }
    
    return gamma, gamma ^ neg
}

func solvePart1(numbers []string) int64 {
    g, e := calculateGammaEps(numbers)
    return g * e
}

func oxygenCrit(zeroCount, oneCount int64) byte {
    if zeroCount <= oneCount {
        return '0'
    }
    return '1'
}

func co2Crit(zeroCount, oneCount int64) byte {
    if zeroCount <= oneCount {
        return '1'
    }
    return '0'
}

func solvePart2(numbers []string) int64 {
    var items []Item

    for _, val := range numbers {
        items = append(items, Item{val, true})
    }

    oxygen := calculateCrit(items, oxygenCrit)
    co2 := calculateCrit(items, co2Crit)

    return oxygen * co2
}

func Solve(file *os.File) (int64, int64) {
    data := parse(file)
    return solvePart1(data), solvePart2(data)
}

