

def char_ord(c):
    return ord(c) - ord('a')


def valid(dp, hashes, design, di):
    if di >= len(design):
        return 1
    if di in dp:
        return dp[di]
    i = di
    h = 0
    found = 0
    while i < len(design):
        h *= 26
        h += char_ord(design[i])
        i += 1
        if h in hashes:
            found += valid(dp, hashes, design, i)
    dp[di] = found
    return found



def phash(pattern):
    h = 0
    for c in pattern:
        h *= 26
        h += char_ord(c)
    return h


def solve(lines):

    patterns = set([p.strip() for p in lines[0].split(',')])
    designs = [lines[i] for i in range(2, len(lines))]
    hashes = set([phash(pattern) for pattern in patterns])

    res1, res2 = 0, 0
    for design in designs:
        dp = {}
        cnt = valid(dp, hashes, design, 0)
        if cnt != 0:
            res1 += 1
        res2 += cnt

    return res1, res2
