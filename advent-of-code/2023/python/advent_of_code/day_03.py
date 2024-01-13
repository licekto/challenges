import re


def get_neigh(symbol_map, coordinates):
    dirs = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]
    neighs = set()
    for i, j in coordinates:
        for di, dj in dirs:
            k = (i + di, j + dj)
            if k in symbol_map:
                neighs.add(k)
    return neighs


def solve(lines):
    nums_map = {}
    symbol_map = {}
    asterisk_set = set()
    for i, line in enumerate(lines):
        for m in re.finditer(r'(\d+)', line):
            k = ()
            v = int(m.group(0))
            for j in range(m.span()[0], m.span()[1]):
                k += ((i, j), )
                symbol_map[(i, j)] = v
            nums_map[k] = v
        for j, c in enumerate(line):
            if not c.isdigit() and c != '.':
                if c == '*':
                    asterisk_set.add((i, j))
                symbol_map[(i, j)] = c

    res1 = 0
    for coordinates, n in nums_map.items():
        if any([not s.isdigit() and s != "." for s in [lines[c[0]][c[1]] for c in get_neigh(symbol_map, coordinates)]]):
            res1 += n

    res2 = 0
    for asterisk in asterisk_set:
        nums = list(set([symbol_map[c] for c in get_neigh(symbol_map, (asterisk, ))]))
        res2 += nums[0] * nums[1] if len(nums) == 2 else 0

    return res1, res2
