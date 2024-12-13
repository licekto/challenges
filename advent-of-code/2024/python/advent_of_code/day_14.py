import re
import sympy


ADD = 10000000000000


class Data:
    def __init__(self, a, b, p):
        self.a = a
        self.b = b
        self.p = p

    def print(self):
        print(f'A=[{self.a[0]}, {self.a[1]}], B=[{self.b[0]}, {self.b[0]}], P=[{self.p[0]}, {self.p[1]}]')


def solve_case(a, b, p):
    x, y = sympy.symbols('x y')
    eq1 = sympy.Eq(a[0] * x + b[0] * y, p[0])
    eq2 = sympy.Eq(a[1] * x + b[1] * y, p[1])
    solution = sympy.solve((eq1, eq2), (x, y), dict=True)

    if not solution[0][x].is_Integer or not solution[0][y].is_Integer:
        return None, None

    return int(solution[0][x]), int(solution[0][y])


def parse(lines, i):
    p_button = re.compile("Button [AB]: X\+([0-9]+), Y\+([0-9]+)")
    p_prize = re.compile("Prize: X=([0-9]+), Y=([0-9]+)")

    m = p_button.findall(lines[i])
    a = (int(m[0][0]), int(m[0][1]))
    m = p_button.findall(lines[i + 1])
    b = (int(m[0][0]), int(m[0][1]))
    m = p_prize.findall(lines[i + 2])
    p = (int(m[0][0]), int(m[0][1]))

    return Data(a, b, p)


def solve(lines):

    res1 = 0
    res2 = 0

    data = []
    i = 0
    while True:
        if i >= len(lines):
            break
        data.append(parse(lines, i))
        i += 4

    for case in data:
        sol = solve_case(case.a, case.b, case.p)
        if sol[0] is not None and sol[1] is not None:
            res1 += sol[0] * 3 + sol[1]

        sol = solve_case(case.a, case.b, (case.p[0] + ADD, case.p[1] + ADD))
        if sol[0] is not None and sol[1] is not None:
            res2 += sol[0] * 3 + sol[1]

    return res1, res2
