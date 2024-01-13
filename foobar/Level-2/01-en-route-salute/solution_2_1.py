
def solution(s):
    cnt = 0
    greet = 0
    for c in s:
        if c == '>':
            cnt += 1
        elif c == '<':
            greet += cnt
    return greet * 2
