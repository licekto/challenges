
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


def print_map(m, s, e):
    for y in range(len(m)):
        for x in range(len(m[0])):
            if (y, x) == s:
                print('S', end='')
            elif (y, x) == e:
                print('E', end='')
            else:
                print(m[y][x], end='')
        print()


def find_cheat(m, p):
    y, x = p
    cv = m[y][x]

    cheats = []
    for sy, sx in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
        ny, nx = y + sy, x + sx
        v = m[ny][nx]
        if v != '#':
            continue

        ny, nx = ny + sy, nx + sx
        if not is_valid(m, (ny, nx)):
            continue
        v = m[ny][nx]
        if type(v) is not int:
            continue
        if v < cv:
            continue
        cheats.append(v - cv - 2)
    return cheats


def find_cheats(m, s, e):
    y, x = s

    cheats = []
    while (y, x) != e:
        cheats.extend(find_cheat(m, (y, x)))
        for sy, sx in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
            ny, nx = y + sy, x + sx
            c = m[y][x]
            v = m[ny][nx]
            if type(v) is int and v > c:
                y, x = ny, nx
                break
    return cheats


def solve(lines):
    print()

    m = [list(line) for line in lines]

    s, e = get_start_end(m)
    eval_map(m, s)

    cheats = find_cheats(m, s, e)
    cheats.sort()
    cheats_count = {x: cheats.count(x) for x in set(cheats)}
    '''
    print(cheats_count)
    print(sum(cheats_count.values()))

    for key, value in sorted(cheats_count.items(), key=lambda x: x[0]):
        print("{} : {}".format(key, value))
    '''
    res1 = 0
    for cheat, count in cheats_count.items():
        if cheat >= 100:
            res1 += count

    return res1, 0
