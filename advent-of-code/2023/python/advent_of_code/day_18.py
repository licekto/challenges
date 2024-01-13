

def parse(lines):
    translate = {'0': 'R', '1': 'D', '2': 'L', '3': 'U'}
    steps1 = []
    steps2 = []
    for line in lines:
        s = line.split(' ')
        steps1.append((s[0], int(s[1])))
        steps2.append((translate[s[2][-2]], int(s[2][2:-2], 16)))
    return steps1, steps2


dirs = {'R': (0, 1), 'L': (0, -1), 'U': (-1, 0), 'D': (1, 0)}


def next_point(p, step):
    return p[0] + dirs[step[0]][0] * step[1], p[1] + dirs[step[0]][1] * step[1]


def to_points(steps):
    points = [(0, 0)]
    prev = 0
    boundaries = 0

    for step in steps:
        points.append(next_point(points[prev], step))
        prev += 1
        boundaries += step[1]

    return points, boundaries


# https://en.wikipedia.org/wiki/Shoelace_formula
def shoelace(points):
    area = 0
    for i, _ in enumerate(points):
        area += points[i][1] * (points[i - 1][0] - points[(i + 1) % len(points)][0])
    return abs(area) // 2


# https://en.wikipedia.org/wiki/Pick%27s_theorem
def picks(area, boundaries):
    return area - boundaries // 2 + 1


def calculate(steps):
    points, boundaries = to_points(steps)
    area = shoelace(points)
    interior = picks(area, boundaries)
    return interior + boundaries


def solve(steps1, steps2):
    return calculate(steps1), calculate(steps2)
