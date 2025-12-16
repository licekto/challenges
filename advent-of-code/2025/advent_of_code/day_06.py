
def solve(lines):
    r1, r2 = 0, 0

    numbers = []
    for i in range(0, len(lines) - 1):
        numbers.append([int(n) for n in lines[i].split(' ') if len(n) != 0])

    ops = [op for op in lines[-1].split(' ') if len(op) != 0]

    print(numbers)
    print(ops)

    for i in range(0, len(numbers[0])):
        sub_res = 1 if ops[i] == '*' else 0
        for j in range(0, len(numbers)):
            if ops[i] == '*':
                sub_res *= numbers[j][i]
            else:
                sub_res += numbers[j][i]
        r1 += sub_res

    last_line = list(lines[-1])
    op_ind = []
    for i in range(0, len(last_line)):
        if last_line[i] == '*' or last_line[i] == '+':
            op_ind.append(i)

    for i in range(0, len(op_ind)):
        n_len = (op_ind[i + 1] if i < len(op_ind) - 1 else len(lines[0])) - op_ind[i] - 1
        sub_res = 1 if ops[i] == '*' else 0
        for j in range(0, n_len):
            n = []
            for k in range(0, len(lines) - 1):
                n.append(lines[k][op_ind[i] + j])
            n = ''.join(n).strip()
            n = int(n)
            if ops[i] == '*':
                sub_res *= n
            else:
                sub_res += n
        r2 += sub_res

    return r1, r2

