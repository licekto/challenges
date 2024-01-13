import graphviz, os


def parse(lines):
    graph = {}

    for line in lines:
        s = line.split(':')
        src = s[0]
        dst = [x.strip() for x in s[1].split()]
        if src not in graph:
            graph[src] = set()
        graph[src].update(dst)

        for d in dst:
            if d not in graph:
                graph[d] = set()
            graph[d].add(src)

    return graph


def to_dot(graph):
    dot = graphviz.Digraph(comment='AoC0-25')

    for s, dst in graph.items():
        dot.node(s, s)
        for d in dst:
            dot.edge(s, d)

    filename = 'graph.dot'
    with open(filename, 'w') as f:
        f.write(str(dot))
    return filename


def to_svg(filename):
    os.system(f'sfdp -x -Goverlap=scale -Tsvg {filename} > graph.svg')


def dfs(graph, visited, init):
    stack = [init]
    cnt = 0

    while len(stack) != 0:
        n = stack.pop()
        if n in visited:
            continue
        visited.add(n)
        cnt += 1

        for neigh in graph[n]:
            stack.append(neigh)

    return cnt


def count(graph):
    visited = set()
    c1 = dfs(graph, visited, list(graph.keys())[0])
    c2_init = None
    for n in graph:
        if n not in visited:
            c2_init = n
    c2 = dfs(graph, visited, c2_init)
    return c1, c2


def solve(graph):
    to_svg(to_dot(graph))
    c1, c2 = count(graph)
    return c1 * c2
