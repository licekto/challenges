
def parse(lines):
    p = []
    for l in lines:
        if 'x' in l:
            x, y = tuple(map(int, l.split(':')[0].split('x')))
            counts = list(map(int, l.split(': ')[1].split()))
            p.append((x, y, counts))

    return p


def solve(lines):
    return sum([1 if 9 * sum(counts) <= m * n else 0 for m, n, counts in parse(lines)])
