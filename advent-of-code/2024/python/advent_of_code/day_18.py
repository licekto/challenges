import heapq
from collections import defaultdict


def solve1(all_obstacles, bound, n):
    obstacles = set(all_obstacles[:n])

    def valid(position):
        return (position[0] in range(bound + 1) and position[1] in range(bound + 1)) and position not in obstacles

    q = []
    heapq.heappush(q, (0, (0, 0)))
    dist = defaultdict(lambda: 10 ** 10)

    result = 0
    while q:
        dst, pos = heapq.heappop(q)

        if pos == (bound, bound):
            result = dst
            break
        if not valid(pos):
            continue

        for nx, ny in [(0, -1), (1, 0), (0, 1), (-1, 0)]:
            next_pos = (pos[0] + nx, pos[1] + ny)
            if dist[next_pos] > dst + 1:
                dist[next_pos] = dst + 1
                heapq.heappush(q, (dst + 1, next_pos))

    return result


def solve2(all_obstacles, bound, n):
    for i in range(n, len(all_obstacles)):
        result = solve1(all_obstacles, bound, i)
        if result == 0:
            return all_obstacles[i - 1]

    return None


def solve(lines, bound, n):
    all_obstacles = list([tuple(map(int, line.split(','))) for line in lines])
    res1 = solve1(all_obstacles, bound, n)

    res2 = solve2(all_obstacles, bound, n)

    return res1, res2
