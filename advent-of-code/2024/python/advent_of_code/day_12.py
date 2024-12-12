
def fill_and_calc(m, visited, x, y):

    if (x, y) in visited:
        return 0, 0

    current = m[y][x]
    visited.add((x, y))
    perimeter = 0
    area = 0
    for sx, sy in [(0, -1), (1, 0), (0, 1), (-1, 0)]:
        nx, ny = x + sx, y + sy
        is_on_map = ny in range(len(m)) and nx in range(len(m[0]))
        if not is_on_map or m[ny][nx] != current:
            perimeter += 1
        elif is_on_map and m[ny][nx] == current and (nx, ny) not in visited:
            sub_perimeter, sub_area = fill_and_calc(m, visited, nx, ny)
            perimeter += sub_perimeter
            area += sub_area

    return perimeter, area + 1


def solve(lines):

    res1 = 0
    res2 = 0

    m = [list(line) for line in lines]

    visited = set()
    for y in range(len(m)):
        for x in range(len(m[0])):
            if (x, y) in visited:
                continue
            perimeter, area = fill_and_calc(m, visited, x, y)
            res1 += perimeter * area

    return res1, res2


lines = [
    'RRRRIICCFF',
    'RRRRIICCCF',
    'VVRRRCCFFF',
    'VVRCCCJFFF',
    'VVVVCJJCFE',
    'VVIVCCJJEE',
    'VVIIICJJEE',
    'MIIIIIJJEE',
    'MIIISIJEEE',
    'MMMISSJEEE'
]

solve(lines)
