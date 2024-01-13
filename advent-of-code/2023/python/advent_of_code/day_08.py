import re
from math import lcm


def parse(lines):
    instructions = list(map(lambda x: 0 if x == 'L' else 1, [c for c in lines[0]]))

    graph = {}
    r = re.compile(r'([A-Z0-9]{3}) = \(([A-Z0-9]{3}), ([A-Z0-9]{3})\)')
    for i in range(2, len(lines)):
        m = r.match(lines[i])
        graph[m.group(1)] = (m.group(2), m.group(3))

    return instructions, graph


def search(instructions, graph, begin, is_end):
    steps = 0
    current = begin
    i = 0
    while True:
        current = graph[current][instructions[i]]
        steps += 1
        if is_end(current):
            break
        i = 0 if i + 1 == len(instructions) else i + 1

    return steps


def search_n(instructions, graph):
    return lcm(*[search(instructions, graph, begin, lambda x: x[2] == 'Z') for begin in [n for n in graph.keys() if n[2] == 'A']])
