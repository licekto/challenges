import re


def get_antennas(m):
    antennas = {}
    for y in range(len(m)):
        for x in range(len(m[0])):
            c = m[y][x]
            if c != '.':
                if c not in antennas:
                    antennas[c] = [(x, y)]
                else:
                    antennas[c].append((x, y))
    return antennas 


def get_antinodes(a1, a2):
    xd, yd = a1[0] - a2[0], a1[1] - a2[1]
    return (a1[0] + xd, a1[1] + yd), (a2[0] - xd, a2[1] - yd)


def is_on_map(m, x, y):
    return x in range(len(m[0])) and y in range(len(m))


def get_harmonics(m, a1, a2):
    xd, yd = a1[0] - a2[0], a1[1] - a2[1]

    harmonics = set()
    harmonics.add((a1[0], a1[1]))
    harmonics.add((a2[0], a2[1]))

    x, y = a1[0] + xd, a1[1] + yd
    while is_on_map(m, x, y):
        harmonics.add((x, y))
        x += xd
        y += yd

    x, y = a2[0] - xd, a2[1] - yd
    while is_on_map(m, x, y):
        harmonics.add((x, y))
        x -= xd
        y -= yd

    return harmonics


def solve(lines):
    
    m = [list(line) for line in lines]
    antennas = get_antennas(m)
   
    antinodes = set() 
    harmonics = set()

    for antenna, coords in antennas.items():
        for i in range(len(coords)):
            for j in range(i + 1, len(coords)):
                (a1x, a1y), (a2x, a2y) = get_antinodes(coords[i], coords[j])
                if is_on_map(m, a1x, a1y):
                    antinodes.add((a1x, a1y))
                if is_on_map(m, a2x, a2y):
                    antinodes.add((a2x, a2y))
                harmonics.update(get_harmonics(m, coords[i], coords[j]))

    res1 = len(antinodes)
    res2 = len(harmonics)

    return res1, res2

