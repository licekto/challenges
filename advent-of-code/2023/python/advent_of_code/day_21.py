from collections import deque


def is_valid(garden, i, j):
    return 0 <= i < len(garden) and 0 <= j < len(garden[0])


def find_start(garden):
    for i in range(len(garden)):
        for j in range(len(garden[0])):
            if garden[i][j] == 'S':
                return i, j
    raise Exception('The S position could not be found')


def bfs(garden, si, sj, steps):
    pos = set()
    visited = set()
    q = deque()
    q.append((si, sj, steps))

    while q:
        i, j, d = q.popleft()

        if not is_valid(garden, i, j) or (i, j, d) in visited or garden[i][j] == '#':
            continue
        visited.add((i, j, d))

        if d % 2 == 0:
            pos.add((i, j))
        if d == 0:
            continue

        for di, dj in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
            q.append((i + di, j + dj, d - 1))

    return len(pos)


def solve_p1(garden, steps):
    return bfs(garden, *find_start(garden), steps)


# https://www.youtube.com/watch?v=9UOMZSL0JTg
def solve_p2(garden):
    steps = 26501365
    if len(garden) != len(garden[0]):
        raise Exception('The garden must be a square')
    size = len(garden)
    if steps % size != size // 2:
        raise Exception('Steps must be divisible by the garden size')
    if (steps - 65) % 131 != 0:
        raise Exception('Steps must be in form of (steps - 65) * 131')

    si, sj = find_start(garden)

    grid_width = steps // size - 1
    odd = (grid_width // 2 * 2 + 1) ** 2
    even = ((grid_width + 1) // 2 * 2) ** 2

    odd_points = bfs(garden, si, sj, size * 2 + 1)
    even_points = bfs(garden, si, sj, size * 2)

    corner_t = bfs(garden, size - 1, sj, size - 1)
    corner_r = bfs(garden, si, 0, size - 1)
    corner_b = bfs(garden, 0, sj, size - 1)
    corner_l = bfs(garden, si, size - 1, size - 1)

    small_tr = bfs(garden, size - 1, 0, size // 2 - 1)
    small_tl = bfs(garden, size - 1, size - 1, size // 2 - 1)
    small_br = bfs(garden, 0, 0, size // 2 - 1)
    small_bl = bfs(garden, 0, size - 1, size // 2 - 1)

    large_tr = bfs(garden, size - 1, 0, size * 3 // 2 - 1)
    large_tl = bfs(garden, size - 1, size - 1, size * 3 // 2 - 1)
    large_br = bfs(garden, 0, 0, size * 3 // 2 - 1)
    large_bl = bfs(garden, 0, size - 1, size * 3 // 2 - 1)

    return odd * odd_points + even * even_points +\
        corner_t + corner_r + corner_b + corner_l +\
        (grid_width + 1) * (small_tr + small_tl + small_br + small_bl) +\
        grid_width * (large_tr + large_tl + large_br + large_bl)
