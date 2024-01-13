from collections import deque
import sys

sys.setrecursionlimit(10000)


def end(trail):
    return len(trail) - 1, len(trail[0]) - 2


def dfs(trail, visited, i, j, depth):
    if (i, j) == end(trail):
        return depth

    best = 0
    for ni, nj, nd in [(i - 1, j, '^'), (i, j + 1, '>'), (i + 1, j, 'v'), (i, j - 1, '<')]:
        nc = trail[ni][nj]
        if nc == '#' or (nc in '>v' and nc != nd) or (ni, nj) in visited:
            continue

        visited.add((ni, nj))
        best = max(best, dfs(trail, visited, ni, nj, depth + 1))
        visited.remove((ni, nj))

    return best


def is_valid(trail, i, j):
    return 0 <= i < len(trail) and 0 <= j < len(trail[0])


def prune(trail):
    nodes = set()
    for i in range(len(trail)):
        for j in range(len(trail[0])):
            if trail[i][j] == '#':
                continue
            neighbors = 0
            for ni, nj in [(i - 1, j), (i, j + 1), (i + 1, j), (i, j - 1)]:
                if is_valid(trail, ni, nj) and trail[ni][nj] != '#':
                    neighbors += 1
            if neighbors > 2:
                nodes.add((i, j))
    nodes.add((0, 1))
    nodes.add((len(trail) - 1, len(trail[0]) - 2))

    graph = {}

    for i, j in nodes:
        for ni, nj in [(i - 1, j), (i, j + 1), (i + 1, j), (i, j - 1)]:
            if is_valid(trail, ni, nj) and trail[ni][nj] != '#':
                q = deque()
                q.append((ni, nj))
                visited = {(i, j)}
                cnt = 0
                while q:
                    ii, jj = q.popleft()
                    visited.add((ii, jj))
                    cnt += 1
                    if (ii, jj) in nodes:
                        if (i, j) not in graph:
                            graph[(i, j)] = set()
                        graph[(i, j)].add((ii, jj, cnt))
                        break
                    for nni, nnj in [(ii - 1, jj), (ii, jj + 1), (ii + 1, jj), (ii, jj - 1)]:
                        if is_valid(trail, nni, nnj) and trail[nni][nnj] != '#' and (nni, nnj) not in visited:
                            q.append((nni, nnj))

    return graph


def dfs2(graph, visited, final, node, depth):
    if node == final:
        return depth

    if node in visited:
        return 0
    visited.add(node)

    best = 0
    for ni, nj, nl in graph[node]:
        best = max(best, dfs2(graph, visited, final, (ni, nj), depth + nl))
    visited.remove(node)

    return best


def solve(trail):
    visited = set()
    res1 = dfs(trail, visited, 0, 1, 0)

    graph = prune(trail)
    visited = set()
    res2 = dfs2(graph, visited, end(trail), (0, 1), 0)

    return res1, res2
