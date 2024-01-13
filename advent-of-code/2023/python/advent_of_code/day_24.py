import itertools
import sympy


def parse(lines_str):
    lines = []
    for line in lines_str:
        s = line.split('@')
        point = tuple([int(x.strip()) for x in s[0].split(',')])
        vector = tuple([int(x.strip()) for x in s[1].split(',')])
        lines.append(Line(point, vector))
    return lines


class Line:
    def __init__(self, p, v):
        self.px = p[0]
        self.py = p[1]
        self.pz = p[2]
        self.vx = v[0]
        self.vy = v[1]
        self.vz = v[2]

    def intersect(self, other):
        xdiff = (self.vx, other.vx)
        ydiff = (self.vy, other.vy)

        def det(a, b):
            return a[0] * b[1] - a[1] * b[0]

        div = det(xdiff, ydiff)
        if div == 0:
            return None

        line1 = ((self.px, self.py), (self.px - self.vx, self.py - self.vy))
        line2 = ((other.px, other.py), (other.px - other.vx, other.py - other.vy))
        d = (det(*line1), det(*line2))
        x = det(d, xdiff) / div
        y = det(d, ydiff) / div

        return x, y

    def has_same_dir(self, x, y):
        return not((x < self.px and self.vx > 0) or (x > self.px and self.vx < 0))

    def __hash__(self):
        return hash(self.p)

    def __str__(self):
        return f'P=[{self.px}, {self.py}, {self.pz}], V=[{self.vx}, {self.vy}, {self.vz}]'

    def __repr__(self):
        return self.__str__()


class Solver:
    def __init__(self, lo, hi):
        self.lo = lo
        self.hi = hi

    def __in_range(self, n):
        return self.lo <= n <= self.hi

    def decide_intersect(self, line1, line2):
        intersection = line1.intersect(line2)
        if intersection is None:
            return False

        x, y = intersection
        if not self.__in_range(x) or not self.__in_range(y):
            return False

        return line1.has_same_dir(x, y) and line2.has_same_dir(x, y)


class Equations:
    def __init__(self):
        self.x1, self.y1, self.z1 = sympy.symbols('x1,y1,z1')
        self.z2, self.y2, self.x2 = sympy.symbols('z2,y2,x2', positive=True, integer=True)
        self.linear_equations = []

    def add(self, line):
        e1 = sympy.Eq((line.px - self.x2) * (self.y1 - line.vy), (line.py - self.y2) * (self.x1 - line.vx))
        e2 = sympy.Eq((line.px - self.x2) * (self.z1 - line.vz), (line.pz - self.z2) * (self.x1 - line.vx))
        self.linear_equations.append(e1)
        self.linear_equations.append(e2)

    def solve(self):
        result = sympy.solve(self.linear_equations, [self.z1, self.z2, self.y1, self.y2, self.x1, self.x2])
        return result[0][1] + result[0][3] + result[0][5]


def solve(hailstones, lo, hi):
    solver = Solver(lo, hi)

    res1 = 0
    for A, B in itertools.combinations(hailstones, 2):
        if solver.decide_intersect(A, B):
            res1 += 1

    equations = Equations()
    for hailstone in hailstones:
        equations.add(hailstone)

    return res1, equations.solve()
