

def parse(lines):
    maps = []
    m = []
    for line in lines:
        if len(line) == 0:
            maps.append(m)
            m = []
            continue
        m.append(line)
    maps.append(m)
    return maps


def find_reflection(bound, compare, allowed_diff):
    i1, i2 = 0, 1
    while i2 < bound:
        diff = 0
        while i2 < bound:
            diff = compare(i1, i2)
            if diff > allowed_diff:
                i1 += 1
                i2 += 1
            else:
                break

        if i2 >= bound:
            return 0

        pi1, pi2 = i1, i2
        perfect = True
        i1 -= 1
        i2 += 1
        while i1 >= 0 and i2 < bound:
            diff += compare(i1, i2)
            if diff > allowed_diff:
                perfect = False
                break
            i1 -= 1
            i2 += 1
        if perfect and diff == allowed_diff:
            return pi2
        i1, i2 = pi1 + 1, pi2 + 1
    return 0


def find_horizontal(m, allowed_diff=0):
    def cmp(i1, i2):
        ss = [m[i1][i] != m[i2][i] for i in range(len(m[0]))]
        res = sum(ss)
        return res
    return find_reflection(len(m), cmp, allowed_diff)


def find_vertical(m, allowed_diff=0):
    def cmp(i1, i2):
        diff = 0
        for i in range(len(m)):
            if m[i][i1] != m[i][i2]:
                diff += 1
        return diff
    return find_reflection(len(m[0]), cmp, allowed_diff)


def solve(maps):

    res1, res2 = 0, 0
    for m in maps:
        res1 += find_vertical(m) + 100 * find_horizontal(m)
        res2 += find_vertical(m, allowed_diff=1) + 100 * find_horizontal(m, allowed_diff=1)

    return res1, res2
