import heapq

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

    dist = {start: 0}
    q = []
    heapq.heappush(q, (0, start, DIRS['>']))
    min_score = 10**10

    while q:
        n = heapq.heappop(q)
        current_cost = n[0]
        x, y = n[1]
        current_dir = n[2]

        if m[y][x] == WALL:
            continue
        if m[y][x] == 'E':
            min_score = min(min_score, current_cost)
            continue

        next_coord = get_next((x, y), current_dir)
        next_cost = current_cost + 1
        if next_coord not in dist or dist[next_coord] > next_cost:
            dist[next_coord] = next_cost
            heapq.heappush(q, (next_cost, next_coord, current_dir))

        rot_dir = rot(current_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 1000
        if next_coord not in dist or dist[next_coord] > next_cost:
            dist[next_coord] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir))

        rot_dir = rot(rot_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 2000
        if next_coord not in dist or dist[next_coord] > next_cost:
            dist[next_coord] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir))

        rot_dir = rot_counter(current_dir)
        next_coord = get_next((x, y), rot_dir)
        next_cost = current_cost + 1 + 1000
        if next_coord not in dist or dist[next_coord] > next_cost:
            dist[next_coord] = next_cost
            heapq.heappush(q, (next_cost, next_coord, rot_dir))

    return min_score

def print_map(m):
    for row in m:
        print(''.join(row))
    #print()


def solve(lines):
    res1, res2 = 0, 0

    m = [list(line) for line in lines]

    x, y = 0, 0
    for i in range(len(lines)):
        pos = lines[i].find('S')
        if pos != -1:
            x, y = pos, i

    res1 = dijkstra(m, (x, y))

    return res1, res2
