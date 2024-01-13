#https://math.stackexchange.com/questions/2052179/how-to-find-sum-i-1n-left-lfloor-i-sqrt2-right-rfloor-a001951-a-beatty-s
sqrt_prec = 4142135623730950488016887242096980785696718753769480731766797379907324784621070388503875343276415727


def init(n):
    return (sqrt_prec * n) // (10 ** 100)


def s(n):
    if n == 1:
        return 1
    if n < 1:
        return 0
    init_n = init(n)
    f1 = n * init_n
    f2 = (n * (n + 1)) / 2
    f3 = init(n) * (init_n + 1) / 2
    f4 = s(init_n)
    return int(f1 + f2 - f3 - f4)


def solution(n):
    return str(s(int(n)))
