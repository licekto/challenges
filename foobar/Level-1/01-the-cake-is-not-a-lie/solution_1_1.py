
def solution(s):
    n = len(s)
    if n == 0:
        return 0
    for i in range(1, n // 2 + 1):
        if n % i == 0:
            substring = s[:i]
            rep = (n // i)
            if substring * rep == s:
                return rep
    return 1
