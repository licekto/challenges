def is_safe1(data):
    up = all(a < b for a, b in zip(data, data[1:]))
    down = all(a > b for a, b in zip(data, data[1:]))
    diff = all(abs(a - b) <= 3 for a, b in zip(data, data[1:]))
    return (up or down) and diff


def is_safe2(data):
    return any(is_safe1(data[:i] + data[i + 1:]) for i in range(len(data)))


def solve(lines):

    res1 = 0
    res2 = 0

    for line in lines:
        data = list(map(int, line.split()))
        is_ok = is_safe1(data)
        res1 = res1 + 1 if is_ok else res1
        res2 = res2 + 1 if is_ok or is_safe2(data) else res2

    return res1, res2
