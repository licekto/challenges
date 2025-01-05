from functools import cache

NUM_MAP = {
    'A': (3, 2),
    '0': (3, 1),
    '1': (2, 0),
    '2': (2, 1),
    '3': (2, 2),
    '4': (1, 0),
    '5': (1, 1),
    '6': (1, 2),
    '7': (0, 0),
    '8': (0, 1),
    '9': (0, 2),
}

DIR_MAP = {
    'A': (0, 2),
    '^': (0, 1),
    '<': (1, 0),
    'v': (1, 1),
    '>': (1, 2)
}


def find_num(code):
    y, x = NUM_MAP['A']

    seq = ''
    for c in code:
        ny, nx = NUM_MAP[c]
        dy, dx = y - ny, x - nx
        cy = '^' if dy > 0 else 'v'
        cx = '<' if dx > 0 else '>'

        if cx == '<':
            if cy == 'v':
                seq += cx * abs(dx) + cy * abs(dy)
            elif cy == '^':
                if y < 3 or nx > 0:
                    seq += cx * abs(dx) + cy * abs(dy)
                else:
                    seq += cy * abs(dy) + cx * abs(dx)
        elif cx == '>':
            if cy == 'v':
                if x > 0 or ny < 3:
                    seq += cy * abs(dy) + cx * abs(dx)
                else:
                    seq += cx * abs(dx) + cy * abs(dy)
            elif cy == '^':
                seq += cy * abs(dy) + cx * abs(dx)
        seq += 'A'
        y, x = ny, nx
    return seq


def find_dir(src, dst):
    y, x = DIR_MAP[src]
    ny, nx = DIR_MAP[dst]

    dy, dx = y - ny, x - nx
    cy = '^' if dy > 0 else 'v'
    cx = '<' if dx > 0 else '>'

    new_seq = ''
    if cx == '<':
        if cy == 'v':
            if nx > 0:
                new_seq += cx * abs(dx) + cy * abs(dy)
            else:
                new_seq += cy * abs(dy) + cx * abs(dx)
        elif cy == '^':
            new_seq += cx * abs(dx) + cy * abs(dy)
    elif cx == '>':
        if cy == 'v':
            new_seq += cy * abs(dy) + cx * abs(dx)
        elif cy == '^':
            if x > 0:
                new_seq += cy * abs(dy) + cx * abs(dx)
            else:
                new_seq += cx * abs(dx) + cy * abs(dy)

    new_seq += 'A'
    return new_seq


@cache
def solve_rec(seq, n):
    if n == 0:
        return len(seq)
    l = 0
    prev = 'A'
    for c in seq:
        subseq = find_dir(prev, c)
        prev = c
        l += solve_rec(subseq, n - 1)

    return l


def solve(codes):

    res1 = 0
    for code in codes:
        seq = find_num(code)
        l = solve_rec(seq, 2)
        res1 += l * int(code[:-1])

    res2 = 0
    for code in codes:
        seq = find_num(code)
        l = solve_rec(seq, 25)
        res2 += l * int(code[:-1])

    return res1, res2
