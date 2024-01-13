

def distance(empty_rows, empty_cols, exp, g1, g2):
    y_range = range(g1[0], g2[0]) if g1[0] < g2[0] else range(g2[0], g1[0])
    x_range = range(g1[1], g2[1]) if g1[1] < g2[1] else range(g2[1], g1[1])

    add = 0
    for r in empty_rows:
        if r in y_range:
            add += 1
    for c in empty_cols:
        if c in x_range:
            add += 1

    diff = abs(g1[0] - g2[0]) + abs(g1[1] - g2[1])
    diff = diff - add + (add * exp)

    return diff


def get_empty_rows(universe):
    empty_rows = []
    for i, row in enumerate(universe):
        if all(c == '.' for c in row):
            empty_rows.append(i)
    return empty_rows


def get_empty_cols(universe):
    empty_cols = []
    for col in range(len(universe[0])):
        is_empty = True
        for row in range(len(universe)):
            if universe[row][col] != '.':
                is_empty = False
                break
        if is_empty:
            empty_cols.append(col)
    return empty_cols


def solve(universe, expand):

    galaxies = []

    for r in range(len(universe)):
        for c in range(len(universe[0])):
            if universe[r][c] == '#':
                galaxies.append((r, c))

    empty_rows = get_empty_rows(universe)
    empty_cols = get_empty_cols(universe)

    res = 0
    for i in range(len(galaxies)):
        for j in range(i + 1, len(galaxies)):
            res += distance(empty_rows, empty_cols, expand, galaxies[i], galaxies[j])

    return res
