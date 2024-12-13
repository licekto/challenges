
def solve(lines):
    left = []
    right = []
    right_cnt = {}
    for line in lines:
        l, r = map(int, line.split())
        left.append(l)
        right.append(r)
        if r not in right_cnt:
            right_cnt[r] = 1
        else:
            right_cnt[r] += 1
    left.sort()
    right.sort()

    sum1 = 0
    sum2 = 0
    for i in range(len(left)):
        sum1 += abs(left[i] - right[i])
        if left[i] in right_cnt:
            sum2 += left[i] * right_cnt[left[i]]

    return sum1, sum2
