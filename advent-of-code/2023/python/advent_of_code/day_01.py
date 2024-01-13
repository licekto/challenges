import re


def solve(lines):
    d = {'one': 1, 'two': 2, 'three': 3, 'four': 4, 'five': 5, 'six': 6, 'seven': 7, 'eight': 8, 'nine': 9}
    r = re.compile(rf'(?=(\d|{"|".join(d.keys())}))')
    res = 0
    for line in lines:
        values = list(map(lambda v: d[v] if not v.isdigit() else v, r.findall(line)))
        res += int(f'{values[0]}{values[-1]}')
    return res
