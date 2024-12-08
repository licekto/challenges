
def compose(n1, n2):
    return int(str(n1) + str(n2))


def find_match(expected, current, operands, idx, do_compose=False):
    if idx >= len(operands):
        return expected == current
    next_op = operands[idx]
    return (
            find_match(expected, current + next_op, operands, idx + 1, do_compose)
            or
            find_match(expected, current * next_op, operands, idx + 1, do_compose)
            or
            (False if not do_compose else find_match(expected, compose(current, next_op), operands, idx + 1, do_compose))
           )


def solve(lines):

    res1 = 0
    res2 = 0

    for line in lines:
        s = line.split(':')
        expected = int(s[0])
        operands = list(map(int, s[1].split()))
        if find_match(expected, operands[0], operands, 1):
            res1 += expected
        if find_match(expected, operands[0], operands, 1, True):
            res2 += expected

    return res1, res2

