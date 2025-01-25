

def to_pin_heights(lines, i):
    pins = []
    for c in range(len(lines[i])):
        cnt = -1
        for r in range(7):
            if lines[i + r][c] == '#':
                cnt += 1
        pins.append(cnt)
    return pins


def fits(key, lock, s):
    for i in range(len(key)):
        if key[i] + lock[i] > s:
            return False
    return True


def solve(lines):

    i = 0

    keys = []
    locks = []
    while i < len(lines):
        if lines[i] == '#' * len(lines[i]):
            locks.append(to_pin_heights(lines, i))
        else:
            keys.append(to_pin_heights(lines, i))
        i += 8

    print(keys)
    print(locks)

    cnt = 0
    for key in keys:
        for lock in locks:
            if fits(key, lock, 5):
                cnt += 1

    return cnt, 0
