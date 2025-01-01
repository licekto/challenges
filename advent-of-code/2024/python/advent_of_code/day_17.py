from collections import defaultdict


class VM:
    def __init__(self, regs, prog):
        self.__regs = regs
        self.__prog = prog
        self.__ip = 0
        self.__out_list = []
        self.__instr = {
            0: self.__adv,
            1: self.__bxl,
            2: self.__bst,
            3: self.__jnz,
            4: self.__bxc,
            5: self.__out,
            6: self.__bdv,
            7: self.__cdv
        }


    def step(self):
        if self.__ip >= len(self.__prog):
            return False
        self.__instr[self.__prog[self.__ip]](self.__prog[self.__ip + 1])
        return True


    def run(self):
        self.__ip = 0
        while self.__ip < len(self.__prog):
            self.__instr[self.__prog[self.__ip]](self.__prog[self.__ip + 1])

    def print_reg(self):
        print(f'- regA={self.__regs[0]}, regB={self.__regs[1]}, regC={self.__regs[2]}, ')


    def __combo(self, op):
        return op if op < 4 else self.__regs[op - 4]


    def __dv(self, op):
        return self.__regs[0] // (2 ** self.__combo(op))


    def __adv(self, op):
        self.__regs[0] = self.__dv(op)
        self.__ip += 2


    def __bxl(self, op):
        self.__regs[1] = self.__regs[1] ^ op
        self.__ip += 2


    def __bst(self, op):
        self.__regs[1] = self.__combo(op) % 8
        self.__ip += 2


    def __jnz(self, op):
        self.__ip = self.__ip + 2 if self.__regs[0] == 0 else op


    def __bxc(self, op):
        self.__regs[1] = self.__regs[1] ^ self.__regs[2]
        self.__ip += 2


    def __out(self, op):
        self.__out_list.append(self.__combo(op) % 8)
        self.__ip += 2


    def __bdv(self, op):
        self.__regs[1] = self.__dv(op)
        self.__ip += 2


    def __cdv(self, op):
        self.__regs[2] = self.__dv(op)
        self.__ip += 2

    def get_out(self):
        return self.__out_list

    def out_as_str(self):
        return ','.join(map(str, self.get_out()))


def solve1(lines):
    regs = []
    for i in range(3):
        regs.append(int(lines[i][len('Register A: '):]))

    vm = VM(regs, list(map(int, lines[4][len('Program: '):].split(','))))
    vm.run()

    res = vm.out_as_str()

    return res


def to_dot_str(dot_str, val, idx):
    bin_val = "{:03b}".format(val)
    for i in range(3):
        if dot_str[i + idx] != '.' and dot_str[i + idx] != bin_val[i]:
            return False
    for i in range(3):
        dot_str[i + idx] = bin_val[i]
    return True


def try_merge(s1, s2):
    new = []
    for i in range(len(s1)):
        if s1[i] == s2[i]:
            new.append(s1[i])
        elif s1[i] == '.' and s2[i] != '.':
            new.append(s2[i])
        elif s1[i] != '.' and s2[i] == '.':
            new.append(s1[i])
        else:
            return []
    return new


def dfs(d, level, a, res):
    if level > 12:
        value = int(''.join(a).replace('.', '0'), 2)
        res.append(value)
    if level >= len(d):
        return

    for i in range(len(d[level])):
        t = d[level][i]
        new = try_merge(a, t)
        if len(new) != 0:
            dfs(d, level + 1, new, res)


def solve2():
    program = [2, 4, 1, 5, 7, 5, 4, 3, 1, 6, 0, 3, 5, 5, 3, 0]

    d = []
    ip = 0
    for instr in program:
        cur = []
        for i in range(0, 8):
            for j in range(0, 8):
                if i ^ 0b101 ^ j ^ 0b110 == instr:
                    values = ['.'] * 48
                    to_dot_str(values, i, 48 - ip * 3 - 3)
                    if to_dot_str(values, j, 48 - ((i ^ 0b101) + (ip * 3)) - 3):
                        cur.append(values)
        d.append(cur)
        ip += 1

    res = []
    dfs(d, 0, ['.'] * 48, res)
    res.sort()

    result = 0
    for a in res:
        vm = VM([a, 0, 0], program)
        vm.run()
        if vm.get_out() == program:
            result = a
            break

    return result

def solve(lines):
    return solve1(lines), solve2()
