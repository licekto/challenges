from copy import *


class Solver:
    def __init__(self, times, times_limit):
        self.__n = len(times)
        self.__times = times
        self.__times_limit = times_limit
        self.__shortest = None
        self.__visited = [False] * len(times)
        self.__best = []

    def __floyd_warshall(self):
        d = deepcopy(self.__times)
        for k in range(len(d)):
            for i in range(len(d)):
                for j in range(len(d)):
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j])
        self.__shortest = d

    def __has_neg_cycle(self):
        for i in range(len(self.__shortest)):
            if self.__shortest[i][i] < 0:
                return True
        return False

    def solve(self):
        self.__floyd_warshall()
        if self.__has_neg_cycle():
            return [n for n in range(self.__n - 2)]

        self.__visited[0] = True

        for i in range(1, self.__n - 1):
            self.__dfs(i, self.__times_limit - self.__shortest[0][i], [])

        self.__best.sort()
        return self.__best

    def __dfs(self, u, time_limit, path):
        n = self.__n
        if time_limit <= -999 or (u == n - 1 and time_limit < 0) or len(self.__best) == n - 2:
            return

        if time_limit >= 0 and u == n - 1:
            if len(path) > len(self.__best):
                self.__best = deepcopy(path)
            return

        if self.__visited[u]:
            return

        self.__visited[u] = True
        path.append(u - 1)

        for v in range(1, n):
            if v == u:
                continue
            self.__dfs(v, time_limit - self.__shortest[u][v], path)

        path.pop()
        self.__visited[u] = False


def solution(times, times_limit):
    solver = Solver(times, times_limit)
    return solver.solve()

