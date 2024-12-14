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


#def is_christmas_tree(robots, w, h):


def calc_quadrants(robots, w, h):
    pass


def solve(lines, w, h):

    print()
    res1 = 0
    res2 = 0

    p = re.compile('p=([0-9]+),([0-9]+) v=(-?[0-9]+),(-?[0-9]+)')

    robots = []
    for line in lines:
        m = p.findall(line)
        robots.append(list(map(int, m[0])))

    #print(robots)
    SEC = 100

    for s in range(SEC):
        #print_robots(robots, w, h)
        for r in range(len(robots)):
            x, y, vx, vy = robots[r]
            robots[r][0] = wrap(x + vx, w)
            robots[r][1] = wrap(y + vy, h)


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

    res1 = q1c * q2c * q3c * q4c

    return res1, res2
