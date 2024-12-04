import re


def solve(lines):

    res1 = 0
    res2 = 0

    p = re.compile("(do\(\))|(don't\(\))|(mul\(([0-9]+),([0-9]+)\))")
    
    do = True
    for line in lines:
        for m in p.findall(line):
            if m[0] == 'do()':
                do = True
            elif m[1] == "don't()":
                do = False
            else:
                mul = int(m[3]) * int(m[4])
                res1 += mul
                if do:
                    res2 += mul

    return res1, res2

