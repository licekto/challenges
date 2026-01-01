from functools import lru_cache


def solve1(lines):
    g, s, r = {l.split(':')[0]: l.split(':')[1].strip().split() for l in lines}, [('you', ['you'])], 0
    while s:
        c, p = s.pop()
        if c == 'out':
            r += 1
            continue
        [s.append((n, p + [n])) for n in g[c] if n not in p]
    return r


def solve2(lines):
    g = {l.split(':')[0]: l.split(':')[1].strip().split() for l in lines}

    @lru_cache(maxsize=None)
    def count_paths(node, mask):
        if node == 'dac':
            mask |= 1
        if node == 'fft':
            mask |= 2
        if node == 'out':
            return 1 if mask == 3 else 0
        return sum([count_paths(n, mask) for n in g[node]])

    return count_paths('svr', 0)
