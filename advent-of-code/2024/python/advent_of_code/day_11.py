import math
from collections import defaultdict


def solve(lines):
    res1 = 0
    res2 = 0
    stones_list = list(map(int, lines[0].split()))
    stones = {}

    for stone in stones_list:
        stones[stone] = 1

    for blink in range(75):
        new_stones = defaultdict(int)
        for stone, count in stones.items():
            if stone == 0:
                new_stones[1] += count
            elif (int(math.log10(stone)) + 1) % 2 == 0:
                str_stone = str(stone)
                half_len = int(len(str_stone) / 2)
                s1 = int(str_stone[:half_len])
                s2 = int(str_stone[half_len:])

                new_stones[s1] += count
                new_stones[s2] += count
            else:
                new_stones[stone * 2024] += count
        stones = new_stones
        if blink == 24:
            for stone, count in stones.items():
                res1 += count

    for stones, count in stones.items():
        res2 += count

    return res1, res2
