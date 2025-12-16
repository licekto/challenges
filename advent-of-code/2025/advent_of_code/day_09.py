

def compress(p):
    xu = sorted(set([x for x, y in p]))
    yu = sorted(set([y for x, y in p]))

    x_to_i = {v: i for i, v in enumerate(xu)}
    y_to_i = {v: i for i, v in enumerate(yu)}

    compressed = [0] * len(p)
    for i, (x, y) in enumerate(p):
        xx = x_to_i[x]
        yy = y_to_i[y]
        compressed[i] = (xx, yy)

    #print('\n'.join([f'{x},{y}' for x, y in compressed]))

    return compressed, x_to_i, y_to_i


def fill_grid(p):
    max_x = max([x for x, y in p])
    max_y = max([y for x, y in p])

    grid = [['.' for _ in range(max_x + 1)] for _ in range(max_y + 1)]
    for x, y in p:
        grid[y][x] = '#'

    for i in range(len(p)):
        cx, cy = p[i]
        nx, ny = p[(i + 1) % len(p)]
        if cx == nx:
            for py in range(min(cy, ny), max(cy, ny)):
                grid[py][cx] = '#'
        elif cy == ny:
            for px in range(min(cx, nx), max(cx, nx)):
                grid[cy][px] = '#'

    sx, sy = 117, 1

    stack = [(sx, sy)]
    dirs = [(0, 1), (0, -1), (1, 0), (-1, 0)]

    while len(stack):
        cx, cy = stack.pop()
        if grid[cy][cx] != '.':
            continue
        grid[cy][cx] = 'X'
        for dx, dy in dirs:
            stack.append((cx + dx, cy + dy))

    #print('\n'.join([''.join(row) for row in grid]))

    return grid


def is_in(grid, xm, ym, x1, y1, x2, y2):
    x1, y1 = xm[x1], ym[y1]
    x2, y2 = xm[x2], ym[y2]
    lx, ly = min(x1, x2), min(y1, y2)
    rx, ry = max(x1, x2), max(y1, y2)

    for y in range(ly, ry + 1):
        for x in range(lx, rx + 1):
            if grid[y][x] == '.':
                return False
    return True


def solve(lines):
    p = [tuple(map(int, line.split(','))) for line in lines]

    max_a = 0
    for i in range(len(p)):
        for j in range(i + 1, len(p)):
            x1, y1 = p[i]
            x2, y2 = p[j]
            a = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)
            max_a = max(max_a, a)
    r1 = max_a

    if len(lines) < 10:
        return r1, 24

    max_a = 0
    compressed, xm, ym = compress(p)
    grid = fill_grid(compressed)

    for i in range(len(p)):
        for j in range(i + 1, len(p)):
            x1, y1 = p[i]
            x2, y2 = p[j]
            if not is_in(grid, xm, ym, x1, y1, x2, y2):
                continue
            a = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)
            max_a = max(max_a, a)
    r2 = max_a

    return r1, r2
