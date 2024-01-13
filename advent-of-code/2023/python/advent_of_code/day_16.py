

class Solver:
    def __init__(self, puzzle):
        self.__puzzle = puzzle
        self.__visited = set()

    def __is_valid(self, state):
        d, i, j = state
        return 0 <= i < len(self.__puzzle) and 0 <= j < len(self.__puzzle[0])

    def __get_next_empty(self, d, i, j):
        for nd, ni, nj in [('>', 0, 1), ('v', 1, 0), ('<', 0, -1), ('^', -1, 0)]:
            if nd == d:
                i += ni
                j += nj
                break
        return [(d, i, j)] if self.__is_valid((d, i, j)) else []

    def __get_next_hor_splitter(self, d, i, j):
        result = []
        if d == '<' or d == '>':
            for nd, ni in [('^', -1), ('v', 1)]:
                n = (nd, i + ni, j)
                if self.__is_valid(n):
                    result.append(n)
        elif d == 'v':
            n = (d, i + 1, j)
            if self.__is_valid(n):
                result.append(n)
        elif d == '^':
            n = (d, i - 1, j)
            if self.__is_valid(n):
                result.append(n)
        else:
            raise Exception(f'Unknown direction: {d}')
        return result

    def __get_next_ver_splitter(self, d, i, j):
        result = []
        if d == '^' or d == 'v':
            for nd, nj in [('<', -1), ('>', 1)]:
                n = (nd, i, j + nj)
                if self.__is_valid(n):
                    result.append(n)
        elif d == '<':
            n = (d, i, j - 1)
            if self.__is_valid(n):
                result.append(n)
        elif d == '>':
            n = (d, i, j + 1)
            if self.__is_valid(n):
                result.append(n)
        else:
            raise Exception(f'Unknown direction: {d}')
        return result

    def __get_next_mirror(self, d, i, j):
        # /
        for cd, nd, ni, nj in [('>', '^', -1, 0), ('v', '<', 0, -1), ('<', 'v', +1, 0), ('^', '>', 0, 1)]:
            if d == cd:
                i += ni
                j += nj
                d = nd
                break
        return [(d, i, j)] if self.__is_valid((d, i, j)) else []

    def __get_next_back_mirror(self, d, i, j):
        # \
        for cd, nd, ni, nj in [('>', 'v', 1, 0), ('v', '>', 0, 1), ('<', '^', -1, 0), ('^', '<', 0, -1)]:
            if d == cd:
                i += ni
                j += nj
                d = nd
                break
        return [(d, i, j)] if self.__is_valid((d, i, j)) else []

    def __get_next(self, d, i, j):
        c = self.__puzzle[i][j]
        if c == '.':
            return self.__get_next_empty(d, i, j)
        elif c == '|':
            return self.__get_next_hor_splitter(d, i, j)
        elif c == '-':
            return self.__get_next_ver_splitter(d, i, j)
        elif c == '/':
            return self.__get_next_mirror(d, i, j)
        elif c == '\\':
            return self.__get_next_back_mirror(d, i, j)
        raise Exception(f'Unknown char={c}')

    def __result(self):
        uniq = set()
        for d, i, j in self.__visited:
            uniq.add((i, j))
        return len(uniq)

    def print(self):
        for i in range(len(self.__puzzle)):
            for j in range(len(self.__puzzle[0])):
                c = '#' if self.__visited[i][j] else '.'
                print(c, end='')
            print()

    def clear(self):
        self.__visited = set()

    def dfs(self, begin_state):
        stack = [begin_state]

        while len(stack) != 0:
            d, i, j = stack.pop()
            if (d, i, j) in self.__visited:
                continue
            self.__visited.add((d, i, j))

            for n in self.__get_next(d, i, j):
                stack.append(n)
        return self.__result()


def solve_p1(puzzle):
    solver = Solver(puzzle)
    result = solver.dfs(('>', 0, 0))
    return result


def solve_p2(puzzle):
    solver = Solver(puzzle)

    result = 0
    for j in range(len(puzzle[0])):
        solver.clear()
        result = max(result, solver.dfs(('v', 0, j)))
        solver.clear()
        result = max(result, solver.dfs(('^', len(puzzle) - 1, j)))

    for i in range(len(puzzle)):
        solver.clear()
        result = max(result, solver.dfs(('>', i, 0)))
        solver.clear()
        result = max(result, solver.dfs(('<', i, len(puzzle[0]) - 1)))

    return result
