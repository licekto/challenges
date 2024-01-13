import re, math


def compute(t, d):
    res = 0
    for i in range(1, t // 2 + 1):
        res += 2 if i * (t - i) > d else 0
    return res - (not t % 2)


def is_int(x):
    return int(x) == x


def compute_fast(t, d):
    delta = math.sqrt(t ** 2 - 4 * d)
    if delta < 0:
        return 0

    min_d = (t - delta) / 2
    if min_d < 0:
        min_d = 0
    min_d = min_d + 1 if is_int(min_d) else math.ceil(min_d)

    max_d = (t + delta) / 2
    max_d = max_d - 1 if is_int(max_d) else math.floor(max_d)
    return max_d - min_d + 1


def solve(lines):
    r = re.compile(r'(\d+)')

    time = list(map(int, r.findall(lines[0])))
    distance = list(map(int, r.findall(lines[1])))

    res1 = 1
    for t, d in zip(time, distance):
        res1 *= compute_fast(t, d)

    return res1, compute_fast(int(''.join(map(str, time))), int(''.join(map(str, distance))))
