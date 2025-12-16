

def comb(line, begin, length, n):
    if length == 0:
        return n

    max_v = 0
    max_i = 0
    for i in range(begin, len(line)):
        if i > len(line) - length:
            break
        v = int(line[i])
        if v > max_v:
            max_v = v
            max_i = i

    return comb(line, max_i + 1, length - 1, n * 10 + max_v)


def solve(lines):

    r1, r2 = 0, 0

    for l in lines:
        r1 += comb(l, 0, 2, 0)
        r2 += comb(l, 0, 12, 0)

    return r1, r2
