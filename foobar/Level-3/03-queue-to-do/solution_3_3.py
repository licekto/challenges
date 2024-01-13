
def fast_xor(n):
    mod = n % 4
    if mod == 0:
        return n
    if mod == 1:
        return 1
    if mod == 2:
        return n + 1
    return 0


def solution(start, length):
    res = 0
    current_row = length
    while current_row > 0:
        res ^= fast_xor(start - 1) ^ fast_xor(start + current_row - 1)
        start += length
        current_row -= 1
    return res

