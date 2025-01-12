from collections import defaultdict


def build_graph(lines):
    graph = defaultdict(set)
    for line in lines:
        nodes = line.strip().split('-')
        graph[nodes[0]].add(nodes[1])
        graph[nodes[1]].add(nodes[0])
    return graph


def is_clique(graph, components):
    for component1 in components:
        for component2 in components:
            if component1 == component2:
                continue
            if component2 not in graph[component1]:
                return False
    return True


def dfs(graph, max_components, component_count, all_cliques, visited, node, clique):
    if node in visited:
        return

    component_count += 1
    visited.add(node)
    clique.add(node)

    if component_count == max_components:
        if is_clique(graph, clique):
            all_cliques.add(tuple(sorted(clique)))
    else:
        for neighbor in graph[node]:
            dfs(graph, max_components, component_count, all_cliques, visited, neighbor, clique)

    clique.remove(node)
    visited.remove(node)


def find_n_clique(graph, n):

    cliques = set()

    for node, neighbors in graph.items():
        clique = set()
        visited = set()
        dfs(graph, n, 0, cliques, visited, node, clique)

    return cliques


def bron_kerbosch(graph, R, P, X):
    if not P and not X:
        yield R
    while P:
        v = P.pop()
        yield from bron_kerbosch(graph, R.union({v}), P.intersection(graph[v]), X.intersection(graph[v]))
        X.add(v)


def find_max(graph):

    all_cliques = list(bron_kerbosch(graph, set(), set(graph.keys()), set()))
    max_clique = set()
    for c in all_cliques:
        if len(c) > len(max_clique):
            max_clique = c

    return ','.join(sorted(max_clique))


def sort_cliques(cliques):

    sorted_cliques = []
    for clique in cliques:
        sorted_cliques.append(sorted(clique))

    return sorted(sorted_cliques)


def eval_cliques(three_cliques):
    res = 0
    for clique in three_cliques:
        for component in clique:
            if component[0] == 't':
                res += 1
                break
    return res


def solve(lines):
    graph = build_graph(lines)
    three_cliques = find_n_clique(graph, 3)

    res1 = eval_cliques(three_cliques)
    res2 = find_max(graph)

    return res1, res2
