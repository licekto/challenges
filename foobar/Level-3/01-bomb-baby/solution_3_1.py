
def solution(x, y):
    m = int(x)
    f = int(y)
    gen = 0

    while m > 1 or f > 1:
        if m < f:
            m, f = f, m
        if f != 1 and m % f == 0:
            return 'impossible'

        div = m // f
        m -= div * f
        gen += div

    return str(gen - 1)
