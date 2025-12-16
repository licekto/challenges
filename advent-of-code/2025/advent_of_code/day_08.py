class DSU:
    def __init__(self, n):
        self.parent = list(range(n))

    def find(self, i):
        if self.parent[i] != i:
            self.parent[i] = self.find(self.parent[i])
        return self.parent[i]

    def union(self, x, y):
        self.parent[self.find(x)] = self.find(y)

    def sizes(self):
        roots = [self.find(i) for i in range(len(self.parent))]
        return sorted({x : roots.count(x) for x in set(roots)}.values(), reverse=True)


def solve(lines, size):
    p = [tuple(map(int, line.split(','))) for line in lines]

    e = []
    for i in range(0, len(p)):
        for j in range(i + 1, len(p)):
            x1, y1, z1 = p[i]
            x2, y2, z2 = p[j]
            d = (x1 - x2) ** 2 + (y1 - y2) ** 2 + (z1 - z2) ** 2
            e.append((i, j, d))
    e = sorted(e, key=lambda x: x[2])

    dsu = DSU(len(p))
    cnt = 0
    conn = 0
    r1, r2 = 0, 0
    for i, j, w in e:
        if dsu.find(i) != dsu.find(j):
            dsu.union(i, j)
            conn += 1
            if conn == len(p) - 1:
                r2 = p[i][0] * p[j][0]
        cnt += 1
        if cnt == size:
            s = dsu.sizes()
            r1 = s[0] * s[1] * s[2]

    return r1, r2
