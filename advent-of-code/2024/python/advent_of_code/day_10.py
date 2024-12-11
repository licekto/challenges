def walk(m, x, y, unique):
    if m[y][x] == 9:
        unique.add((x, y))
        return 1
    cnt = 0
    for sy, sx in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
        nx, ny = x + sx, y + sy
        if ny not in range(len(m)) or nx not in range(len(m[0])):
            continue
        if m[ny][nx] == m[y][x] + 1:
            cnt += walk(m, nx, ny, unique)
    return cnt


def solve(lines):

    res1 = 0
    res2 = 0

    m = [list(map(int, row)) for row in lines]

    for y in range(len(m)):
        for x in range(len(m[0])):
            if m[y][x] == 0:
                unique = set()
                res2 += walk(m, x, y, unique)
                res1 += len(unique)

    return res1, res2
