import sys


def solution(xs):
    if len(xs) == 1:
        return str(xs[0])

    neg_count = 0
    neg_max = -1001

    res = None
    for x in xs:
        if x != 0:
            res = x if res is None else res * x

        if x < 0:
            neg_count += 1
            neg_max = x if x > neg_max else neg_max

    if res is not None and neg_count != 0 and neg_count % 2 == 1:
        res //= neg_max

    return f'{int(res if res is not None else 0)}'

