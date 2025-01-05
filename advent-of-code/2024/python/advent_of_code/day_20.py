from collections import Counter

def is_valid(m, p):
    y, x = p
    return y in range(len(m)) and x in range(len(m[0]))


def get_start_end(m):
    start, end = None, None
    for y in range(len(m)):
        for x in range(len(m[0])):
            if m[y][x] == 'S':
                start = y, x
            if m[y][x] == 'E':
                end = y, x
    return start, end


def eval_map(m, start):
    y, x = start
    m[y][x] = 0
    d = 1

    end = False
    while not end:
        for ny, nx in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
            ty, tx = y + ny, x + nx
            if not is_valid(m, (ty, tx)) or m[ty][tx] == '#' or type(m[ty][tx]) is int:
                continue
            v = m[ty][tx]
            m[ty][tx] = d
            if v == 'E':
                end = True
                break
            y, x, = ty, tx
            d += 1
            break


def manhattan(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


def find_cheat(m, s, n):
    y, x = s
    cheats = []

    for iy in range(y - n, y + n + 1):
        for ix in range(x - n, x + n + 1):
            if not is_valid(m, (iy, ix)) or m[iy][ix] == '#':
                continue
            distance = manhattan(s, (iy, ix))
            orig = m[iy][ix] - m[y][x]
            if distance <= n and orig > 0 and orig - distance > 0:
                cheats.append(orig - distance)
    return cheats


def find_cheats(m, s, e, n):
    y, x = s

    cheats = []

    while (y, x) != e:
        cheats.extend(find_cheat(m, (y, x), n))
        for sy, sx in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
            ny, nx = y + sy, x + sx
            c = m[y][x]
            v = m[ny][nx]
            if type(v) is int and v > c:
                y, x = ny, nx
                break
    return cheats


def get_result(m, s, e, n):
    cheats = find_cheats(m, s, e, n)
    return sum([count if cheat >= 100 else 0 for cheat, count in Counter(cheats).items()])


def solve(lines):
    m = [list(line) for line in lines]
    s, e = get_start_end(m)
    eval_map(m, s)

    return get_result(m, s, e, 2), get_result(m, s, e, 20)
