
def solve(lines):

    dial, res1, res2 = 50, 0, 0
    for l in lines:
        d = l[0]
        n = int(l[1:])
        s = -1 if d == 'L' else 1

        l = (n % 100) * s
        h = abs(int(n / 100) * 100)

        dial += h
        res2 += abs(int(dial / 100))
        dial = dial % 100

        prev = dial
        dial += l
        res2 += 1 if prev != 0 and dial < 0 or dial > 100 else 0
        dial = dial % 100

        res1 += 1 if dial == 0 else 0
        res2 += 1 if dial == 0 else 0
        
    return res1, res2
