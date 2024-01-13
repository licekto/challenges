import re
from functools import reduce


def solve(lines):
    constraints = {'red': 12, 'green': 13, 'blue': 14}
    res1, res2 = 0, 0

    for line in lines:
        game, sets_str = line.split(':')
        game_id = int(re.match(r'Game (\d+)', game).group(1))
        sets = [[draw.strip().split() for draw in set_str.split(',')] for set_str in sets_str.split(';')]

        possible = True
        max_cubes = {'red': 0, 'green': 0, 'blue': 0}

        for s in sets:
            for n, c in s:
                n = int(n)
                max_cubes[c] = max(max_cubes[c], n)
                possible = False if n > constraints[c] else possible
        res1 += game_id if possible else 0
        res2 += reduce((lambda x, y: x * y), max_cubes.values())

    return res1, res2
