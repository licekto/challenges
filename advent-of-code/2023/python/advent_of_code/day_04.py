import re


def solve(lines):
    res1 = 0
    r = re.compile(r'Card\s+(\d+):((?:\s+\d+)+) \|((?:\s+\d+)+)')
    copies = [1] * len(lines)
    for line in lines:
        m = r.match(line)
        card_id = int(m.group(1))
        win = set(map(int, m.group(2).split()))
        my = set(map(int, m.group(3).split()))
        intersection = len(win.intersection(my))
        res1 += 0 if intersection == 0 else 2**(intersection - 1)
        for i in range(intersection):
            copies[card_id + i] += copies[card_id - 1]

    return res1, sum(copies)
