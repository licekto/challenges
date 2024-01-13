import re
from .interval_map import *


class Almanac:
    def __init__(self, maps):
        self.__maps = maps
        self.maps = maps

    def process_seed(self, s):
        for interval_map in self.__maps:
            k, v = interval_map.get_interval(s)
            s += v - k
            print(f'(s={s}), ', end='')
        print()
        return s


def parse(lines):
    seeds = list(map(int, re.findall(r'(\d+)', lines[0])))

    maps = []
    i = 1
    while i < len(lines):
        i += 2
        interval_map = IntervalMap()
        interval_map[0] = 0
        while i < len(lines) and len(lines[i]) != 0:
            destination, source, length = tuple(map(int, re.findall(r'(\d+)', lines[i])))
            interval_map[source] = destination
            if source + length not in interval_map:
                interval_map[source + length] = source + length
            i += 1
        maps.append(interval_map)

    return seeds, Almanac(maps)


MAX = 10**20


def solve(seeds, almanac):
    res1 = min(map(almanac.process_seed, seeds))

    print()
    [print(m) for m in almanac.maps]

    interval_map = IntervalMap()
    for interval, length in [(seeds[i], seeds[i + 1]) for i in range(0, len(seeds), 2)]:
        interval_map[interval] = length
    print(interval_map)

    rrr = MAX

    for seed_interval, seed_interval_size in interval_map.items():
        int_map = IntervalMap()
        int_map[seed_interval] = seed_interval_size
        for m in almanac.maps:
            new_interval_map = IntervalMap()
            for ss, ll in int_map.items():
                n = m.get_interval_node(ss)
                k, v = n.key, n.value
                ni = ss + v - k
                map_interval_size = MAX if n.next is None else n.next.key - ni
                if map_interval_size >= ll:
                    new_interval_map[ni] = ll
                else:
                    subtract = n.next.key - ss
                    new_interval_map[ni] = subtract
                    ll -= subtract
                    n = n.next

                    if n.next is not None:
                        new_interval_map[n.next.value] = ll - subtract
                    else:
                        new_interval_map[n.value] = ll

            int_map = new_interval_map

        rrr = min(rrr, iter(int_map).get()[0])

    return res1, rrr
