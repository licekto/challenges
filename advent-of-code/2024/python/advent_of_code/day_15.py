

BOX = 'O'
B1 = '['
B2 = ']'
WALL = '#'
EMPTY = '.'
ROBOT = '@'
DIRS = {'^': (0, -1, '^'), '>': (1, 0, '>'), 'v': (0, 1, 'v'), '<': (-1, 0, '<')}


def get_next(coord, d):
    return coord[0] + d[0], coord[1] + d[1]


def move_boxes(m, d, coord):
    nx, ny = get_next(coord, d)
    x, y = coord
    c = m[ny][nx]

    if c == WALL:
        return

    if c == BOX:
        move_boxes(m, d, (nx, ny))

    if m[ny][nx] == EMPTY:
        m[ny][nx] = BOX
        m[y][x] = EMPTY
        return


def move_ext_boxes(m, d, coord):
    nx1, ny1 = get_next(coord, d)

    c = m[ny1][nx1]
    if c == WALL:
        return
    x, y = coord

    if d[2] == '<' or d[2] == '>':
        if (d[2] == '<' and m[ny1][nx1] != B1) or (d[2] == '>' and m[ny1][nx1] != B2):
            raise Exception(f'Invalid box at [{nx1}, {ny1}]')

        nx2, ny2 = get_next((nx1, ny1), d)
        c = m[ny2][nx2]
        if c == WALL:
            return
        if c == B1 or c == B2:
            move_ext_boxes(m, d, (nx2, ny2))

        c = m[ny2][nx2]
        if c == EMPTY:
            m[ny2][nx2] = m[ny1][nx1]
            m[ny1][nx1] = m[y][x]
            m[y][x] = EMPTY
    else:
        b1x, b1y = (x, y) if m[y][x] == '[' else (x - 1, y)
        b2x, b2y = (x + 1, y) if m[y][x] == '[' else (x, y)

        next_y = ny1

        if m[next_y][b1x] == '[' and m[next_y][b2x] == ']':
            move_ext_boxes(m, d, (b1x, next_y))
        if m[next_y][b1x] == ']' and m[next_y][b2x] == '[':
            if can_move(m, d, b1x, next_y) and can_move(m, d, b2x, next_y):
                move_ext_boxes(m, d, (b1x, next_y))
                move_ext_boxes(m, d, (b2x, next_y))
        if m[next_y][b1x] == ']' and m[next_y][b2x] == EMPTY:
            move_ext_boxes(m, d, (b1x, next_y))
        if m[next_y][b1x] == EMPTY and m[next_y][b2x] == '[':
            move_ext_boxes(m, d, (b2x, next_y))


        sx, sy = (nx1 + 1, ny1) if m[y][x] == '[' else (nx1 - 1, ny1)
        if m[next_y][b1x] == EMPTY and m[next_y][b2x] == EMPTY:
            sx1 = x + 1 if m[y][x] == '[' else x - 1
            m[ny1][nx1] = m[y][x]
            m[sy][sx] = m[y][sx1]
            m[y][x] = EMPTY
            m[y][sx1] = EMPTY


def can_move(m, d, x, y):
    nx, ny = get_next((x, y), d)
    b1x, b1y = (x, y) if m[y][x] == '[' else (x - 1, y)
    b2x, b2y = (x + 1, y) if m[y][x] == '[' else (x, y)

    if m[ny][b1x] == EMPTY and m[ny][b2x] == EMPTY:
        return True
    if m[ny][b1x] == WALL or m[ny][b2x] == WALL:
        return False
    if m[ny][b1x] == '[' or m[ny][b2x] == ']':
        return can_move(m, d, b1x, ny)
    if m[ny][b1x] == ']' and m[ny][b2x] == '[':
        return can_move(m, d, b1x, ny) and can_move(m, d, b2x, ny)
    if m[ny][b1x] == ']' and m[ny][b2x] == EMPTY:
        return can_move(m, d, b1x, ny)
    if m[ny][b1x] == EMPTY and m[ny][b2x] == '[':
        return can_move(m, d, b2x, ny)

def get_result(m):
    cnt = 0
    for y in range(len(m)):
        for x in range(len(m[0])):
            if m[y][x] != BOX:
                continue
            cnt += y * 100 + x
    return cnt


def get_result_ext(m):
    cnt = 0
    for y in range(len(m)):
        for x in range(len(m[0])):
            if m[y][x] != B1:
                continue
            cnt += y * 100 + x
    return cnt


def print_map(m):
    for row in m:
        print(''.join(row))
    #print()


def extend_map(m):
    new_m = []
    for row in m:
        new_row = []
        for c in row:
            if c == WALL:
                new_row.extend([WALL, WALL])
            if c == BOX:
                new_row.extend([B1, B2])
            if c == EMPTY:
                new_row.extend([EMPTY, EMPTY])
            if c == ROBOT:
                new_row.extend([ROBOT, EMPTY])
        new_m.append(new_row)
    return new_m


def solve(lines):

    m = []
    rx, ry = 0, 0

    i = 0
    for line in lines:
        if len(line) == 0:
            break
        m.append(list(line))
        pos = line.find('@')
        if pos != -1:
            rx, ry = pos, i
        i += 1
    em = extend_map(m)

    moves = []
    for li in range(i + 1, len(lines)):
        moves.extend(list(lines[li]))

    for move in moves:
        d = DIRS[move]
        nx, ny = get_next((rx, ry), d)
        c = m[ny][nx]
        if c == WALL:
            continue
        if c == BOX:
            move_boxes(m, d, (nx, ny))
        if m[ny][nx] == EMPTY:
            m[ny][nx] = ROBOT
            m[ry][rx] = EMPTY
            rx, ry = nx, ny
        #print_map(m)

    res1 = get_result(m)

    found = False
    for y in range(len(em)):
        for x in range(len(em[0])):
            if em[y][x] == ROBOT:
                rx, ry = x, y
                found = True
                break
        if found:
            break

    for m in range(len(moves)):
        d = DIRS[moves[m]]

        nx, ny = get_next((rx, ry), d)
        c = em[ny][nx]
        if c == WALL:
            continue
        if c == B1 or c == B2:
            move_ext_boxes(em, d, (nx, ny))
        if em[ny][nx] == EMPTY:
            em[ny][nx] = ROBOT
            em[ry][rx] = EMPTY
            rx, ry = nx, ny

    res2 = get_result_ext(em)

    return res1, res2
