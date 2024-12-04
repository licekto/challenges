
def find_xmas(xmap, y, x):
    
    p = 'MAS'
    cnt = 0

    if xmap[y][x] != 'X':
        return 0
    for xd, yd in [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]:
        xi, yi = x, y
        found = True
        for c in p:
            xi += xd
            yi += yd
            if xi < 0 or xi >= len(xmap[0]) or yi < 0 or yi >= len(xmap):
                found = False
                break
            if xmap[yi][xi] != c:
                found = False
                break
        cnt += 1 if found else 0
    return cnt


def find_x_mas(xmap, x, y):

    if x < 1 or x >= len(xmap[0]) - 1 or y < 1 or y >= len(xmap) - 1 or xmap[y][x] != 'A':
        return False

    x11 = xmap[y - 1][x - 1]
    x12 = xmap[y + 1][x + 1]
    x21 = xmap[y + 1][x - 1]
    x22 = xmap[y - 1][x + 1]


    xc1 = (x11 == 'M' and x12 == 'S') or (x11 == 'S' and x12 == 'M')
    xc2 = (x21 == 'M' and x22 == 'S') or (x21 == 'S' and x22 == 'M')

    return 1 if xc1 and xc2 else 0


def solve(xmap):

    res1 = 0
    res2 = 0

    for y in range(len(xmap)):
        for x in range(len(xmap[0])):
            res1 += find_xmas(xmap, x, y)
            res2 += find_x_mas(xmap, x, y)

    return res1, res2

