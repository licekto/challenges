
def is_rep(v):
    s = str(v)
    if len(s) % 2 != 0:
        return False
    h = len(s) // 2
    return s[:h] == s[h:]


def is_seq(v):
    s = str(v)
    sl = len(s)
    for l in range(2, sl + 1):
        if sl % l != 0:
            continue

        chunk_size = len(s) // l
        parts = [s[i:i + chunk_size] for i in range(0, len(s), chunk_size)]
        if len(set(parts)) == 1:
            return True
    return False


def solve(lines):

    r1, r2 = 0, 0
    for r in lines[0].split(','):
        l, h = map(int, tuple(r.split('-')))
        for i in range(l, h + 1):
            r1 += i if is_rep(i) else 0
            r2 += i if is_seq(i) else 0

    return r1, r2

