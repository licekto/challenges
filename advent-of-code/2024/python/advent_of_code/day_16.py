import heapq
from collections import defaultdict

WALL = '#'
EMPTY = '.'

DIRS = {'^': (0, -1, '^'), '>': (1, 0, '>'), 'v': (0, 1, 'v'), '<': (-1, 0, '<')}

MAX = 10**10

def rot(d):
    return DIRS[{'^': '>', '>': 'v', 'v': '<', '<': '^'}[d[2]]]


def rot_counter(d):
    return DIRS[{'^': '<', '<': 'v', 'v': '>', '>': '^'}[d[2]]]


def get_next(coord, d):
    return coord[0] + d[0], coord[1] + d[1]


def dijkstra(m, start):
    dist = defaultdict(lambda: 10**10)
    q = []
    heapq.heappush(q, (0, start, DIRS['>'], [start]))
    min_score = 10**10

    path = []

    while q:
        n = heapq.heappop(q)
        current_cost = n[0]
        x, y = n[1]
        current_dir = n[2]
        cp = n[3]

        if m[y][x] == WALL:
            continue
        if m[y][x] == 'E' and current_cost <= min_score:
            min_score = current_cost
            path += cp

        next_coord = get_next((x, y), current_dir)
        next_cost = current_cost + 1
        if dist[next_coord, current_dir[2]] >= next_cost:
            dist[next_coord, current_dir[2]] = next_cost
            heapq.heappush(q, (next_cost, next_coord, current_dir, cp + [next_coord]))

        rot_dir = rot(current_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 1000
        if dist[next_coord, rot_dir[2]] >= next_cost:
            dist[next_coord, rot_dir[2]] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir, cp + [next_coord]))

        rot_dir = rot(rot_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 2000
        if dist[next_coord, rot_dir[2]] >= next_cost:
            dist[next_coord, rot_dir[2]] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir, cp + [next_coord]))

        rot_dir = rot_counter(current_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 1000
        if dist[next_coord, rot_dir[2]] >= next_cost:
            dist[next_coord, rot_dir[2]] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir, cp + [next_coord]))

    return min_score, len(set(path))

def solve(lines):
    m = [list(line) for line in lines]

    sx, sy = 0, 0
    for i in range(len(lines)):
        pos = lines[i].find('S')
        if pos != -1:
            sx, sy = pos, i

    res1, res2 = dijkstra(m, (sx, sy))

    return res1, res2
