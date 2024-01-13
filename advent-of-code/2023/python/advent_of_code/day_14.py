

def to_list(lines):
    return list(map(list, lines))


def tilt(m):
    for i in range(1, len(m)):
        for j in range(len(m[0])):
            if m[i][j] != 'O':
                continue
            k = i - 1
            while k >= 0 and m[k][j] == '.':
                k -= 1
            k += 1
            m[i][j] = '.'
            m[k][j] = 'O'


def calculate(m):
    n = len(m)
    res = 0
    for i in range(len(m)):
        for j in range(len(m[0])):
            if m[i][j] == 'O':
                res += n
        n -= 1
    return res


def solve_part1(m):
    tilt(m)
    return calculate(m)


def cycle(m):
    for i in range(4):
        tilt(m)
        m = list(map(list, zip(*m[::-1])))
    return m


def solve_part2(m):
    total_count = 1000000000
    map_set = {}
    i = 0

    while True:
        m = cycle(m)
        hashable = tuple(map(tuple, m))
        if hashable in map_set:
            period = i - map_set[hashable]
            break
        map_set[hashable] = i
        i += 1

    rem = (total_count - i - 1) % period

    for i in range(rem):
        m = cycle(m)

    return calculate(m)
