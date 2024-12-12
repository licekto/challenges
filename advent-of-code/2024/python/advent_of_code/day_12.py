

def is_on_map(m, x, y):
    return y in range(len(m)) and x in range(len(m[0]))


def fill_and_calc(m, visited, x, y):

    if (x, y) in visited:
        return 0, 0

    current = m[y][x]
    visited.add((x, y))
    perimeter = 0
    area = 0
    for sx, sy in [(0, -1), (1, 0), (0, 1), (-1, 0)]:
        nx, ny = x + sx, y + sy
        if not is_on_map(m, nx, ny) or m[ny][nx] != current:
            perimeter += 1
        elif is_on_map(m, nx, ny) and m[ny][nx] == current and (nx, ny) not in visited:
            sub_perimeter, sub_area = fill_and_calc(m, visited, nx, ny)
            perimeter += sub_perimeter
            area += sub_area

    return perimeter, area + 1


def get_sides(area):
    s = 0

    for x, y in area:
        s += (x - 1, y) not in area and (x, y - 1) not in area
        s += (x + 1, y) not in area and (x, y - 1) not in area
        s += (x - 1, y) not in area and (x, y + 1) not in area
        s += (x + 1, y) not in area and (x, y + 1) not in area

        s += (x - 1, y) in area and (x, y - 1) in area and (x - 1, y - 1) not in area
        s += (x + 1, y) in area and (x, y - 1) in area and (x + 1, y - 1) not in area
        s += (x - 1, y) in area and (x, y + 1) in area and (x - 1, y + 1) not in area
        s += (x + 1, y) in area and (x, y + 1) in area and (x + 1, y + 1) not in area

    return s


def solve(lines):

    res1 = 0
    res2 = 0

    m = [list(line) for line in lines]

    visited = set()
    for y in range(len(m)):
        for x in range(len(m[0])):
            if (x, y) in visited:
                continue
            current_visited = set()
            perimeter, area = fill_and_calc(m, current_visited, x, y)
            sides = get_sides(current_visited)
            res1 += area * perimeter
            res2 += area * sides
            visited.update(current_visited)

    return res1, res2
