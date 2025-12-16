

def check(m, x, y):
    if m[y][x] == '.':
        return 8
    cnt = 0
    for i, j in [(0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1)]:
        xi, yj = x + i, y + j
        if 0 <= xi < len(m[0]) and 0 <= yj < len(m):
            if m[yj][xi] == '@':
                cnt += 1
    return cnt


def part_1(lines):
    r = 0

    for y in range(0, len(lines)):
        for x in range(0, len(lines[0])):
            if check(lines, x, y) < 4:
                r += 1
    return r


def part_2(lines):
    r = 0

    removed_any = True

    while removed_any:
        removed_any = False
        for y in range(0, len(lines)):
            for x in range(0, len(lines[0])):
                if check(lines, x, y) < 4:
                    r += 1
                    removed_any = True
                    lines[y][x] = '.'
    return r


def solve(lines):

    m = [list(elem) for elem in [row for row in lines]]

    r1 = part_1(m)
    r2 = part_2(m)

    return r1, r2

