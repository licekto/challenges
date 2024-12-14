import re


def wrap(n, b):
    return b + n if n < 0 else n % b


def print_robots(robots, w, h):
    for i in range(h):
        for j in range(w):
            c = 0
            for x, y, _, _ in robots:
                if x == j and y == i:
                    c += 1
            print(f'{c if c != 0 else "."}', end='')
        print()
    print()


def calc_quadrants(robots, w, h):
    q1x, q1y = range(int(w / 2)), range(int(h / 2))
    q2x, q2y = range(int(w / 2) + 1, w), range(int(h / 2))
    q3x, q3y = range(int(w / 2)), range(int(h / 2) + 1, h)
    q4x, q4y = range(int(w / 2) + 1, w), range(int(h / 2) + 1, h)

    q1c, q2c, q3c, q4c = 0, 0, 0, 0
    for x, y, _, _ in robots:
        q1c += 1 if x in q1x and y in q1y else 0
        q2c += 1 if x in q2x and y in q2y else 0
        q3c += 1 if x in q3x and y in q3y else 0
        q4c += 1 if x in q4x and y in q4y else 0

    return q1c * q2c * q3c * q4c


def dfs(x, y, graph, w, h, visited):
    if x not in range(w) or y not in range(h) or (x, y) in visited or (x, y) not in graph:
        return 0

    visited.add((x, y))
    comp = 1
    dirs = [(0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1)]
    for sx, sy in dirs:
        comp += dfs(x + sx, y + sy, graph, w, h, visited)

    return comp


def connected(robots, w, h):
    graph = set()
    for x, y, _, _ in robots:
        graph.add((x, y))

    visited = set()
    max_comp = 0

    for x, y, _, _ in robots:
        max_comp = max(max_comp, dfs(x, y, graph, w, h, visited))

    return max_comp


def solve(lines, w, h):
    res1 = 0
    res2 = 0

    p = re.compile('p=([0-9]+),([0-9]+) v=(-?[0-9]+),(-?[0-9]+)')

    robots = []
    for line in lines:
        m = p.findall(line)
        robots.append(list(map(int, m[0])))

    SEC = 100

    max_comp = 0
    for s in range(1, 10000):
        for r in range(len(robots)):
            x, y, vx, vy = robots[r]
            robots[r][0] = wrap(x + vx, w)
            robots[r][1] = wrap(y + vy, h)

        if s == SEC:
            res1 = calc_quadrants(robots, w, h)

        comp = connected(robots, w, h)
        if comp > max_comp:
            max_comp = comp
            res2 = s
            #print(f'sec={s}, comp={comp}')
            #print_robots(robots, w, h)


    return res1, res2
