import re
import sys
from dataclasses import dataclass

class Gate:
    def __init__(self, op, in1, in2, out):
        self.op = op
        self.in1 = in1
        self.in2 = in2
        self.out = out

    def exec(self, in1, in2):
        if self.op == 'AND':
            return in1 and in2
        if self.op == 'OR':
            return in1 or in2
        if self.op == 'XOR':
            return in1 != in2


def part1(gates, wires):
    run = True
    while run:
        run = False
        for gate in gates:
            if gate.in1 in wires and gate.in2 in wires and gate.out not in wires:
                wires[gate.out] = gate.exec(wires[gate.in1], wires[gate.in2])
                run = True

    return int(''.join([v for k, v in sorted([(k, '1' if v else '0') for k, v in wires.items() if k[0] == 'z'], reverse=True)]), 2)


def part2_sol(data):

    @dataclass
    class Connection:
        ins: list[str]
        op: str
        out: str

        def __str__(self) -> str:
            return f"{self.out} = {self.ins[0]} {self.op} {self.ins[1]}"

        def __eq__(self, other) -> bool:
            return self.ins == other.ins and self.op == other.op

    operations = {
        "OR": lambda x1, x2: x1 | x2,
        "AND": lambda x1, x2: x1 & x2,
        "XOR": lambda x1, x2: x1 ^ x2,
    }

    def run_wire(w: str):
        if w in init:
            return init[w]
        conn = wire_map[w]
        return operations[conn.op](run_wire(conn.ins[0]), run_wire(conn.ins[1]))

    data = '\n'.join(data)
    #print(data)

    init_pairs = re.findall(r"(.{3}): ([01])", data)
    init = {k: int(v) for k, v in init_pairs}
    map_str = data.split("\n\n")[1].splitlines()

    wire_map = {}
    for line in map_str:
        in1, op, in2, _, out = line.strip().split(" ")
        wire_map[out] = Connection([in1, in2], op, out)

    def run_wire2(w: str, init: dict[list[int]]) -> int:
        if res := re.match(r"(x|y)(\d{2})", w):
            var, num = res.groups()
            return init[var][int(num)]
        conn = wire_map[w]
        return operations[conn.op](run_wire2(conn.ins[0], init), run_wire2(conn.ins[1], init))

    def make_wire(var, num):
        return var + str(num).zfill(2)

    def validate(n: int) -> bool:
        for x in range(2):
            for y in range(2):
                for c in range(2):
                    init_x = [0] * (44 - n) + [x]
                    init_y = [0] * (44 - n) + [y]
                    if n > 0:
                        init_x += [c] + [0] * (n - 1)
                        init_y += [c] + [0] * (n - 1)
                    elif c > 0:
                        continue
                    init_x, init_y = list(reversed(init_x)), list(reversed(init_y))
                    z = run_wire2(make_wire("z", n), {"x": init_x, "y": init_y})
                    if z != (x + y + c) % 2:
                        return False
        return True

    def find_wire(op: str | None = None, in1: str | None = None, in2: str | None = None):
        for wire in wire_map.values():
            if op and op != wire.op:
                continue
            if in1 and in1 not in wire.ins:
                continue
            if in2 and in2 not in wire.ins:
                continue
            return wire

    def swap(w1: str, w2: str) -> None:
        wire_map[w1], wire_map[w2] = wire_map[w2], wire_map[w1]

    def fix_bit_n(n: int) -> list[str]:
        """
        zn = nxor XOR m1
        nxor = xn XOR yn
        m1 = m2 OR prevand
        prevand = xn-1 AND yn-1
        m2 = prevxor AND (something else from prev)
        prevxor = xn-1 XOR yn-1

        know m2 is good or would have crashed on prev validation
        """
        print(f"Issue with n = {n}")
        prevand = find_wire(op="AND", in1=make_wire("x", n - 1), in2=make_wire("y", n - 1))
        prevxor = find_wire(op="XOR", in1=make_wire("x", n - 1), in2=make_wire("y", n - 1))
        m2 = find_wire(op="AND", in1=prevxor.out)
        m1 = find_wire(op="OR", in1=m2.out, in2=prevand.out)
        nxor = find_wire("XOR", in1=make_wire("x", n), in2=make_wire("y", n))
        zn = find_wire(op="XOR", in1=nxor.out, in2=m1.out)
        if zn is None:
            zn = wire_map[make_wire("z", n)]
            to_swap = list(set(zn.ins) ^ set([nxor.out, m1.out]))
        if zn.out != make_wire("z", n):
            to_swap = [make_wire("z", n), zn.out]
        swap(*to_swap)
        return to_swap

    part2 = []
    for i in range(45):
        if validate(i):
            continue
        part2.extend(fix_bit_n(i))

    return ",".join(sorted(part2))


def solve(lines):

    wires = {}

    i = 0
    while len(lines[i]):
        wire, signal = lines[i].split(':')
        signal = signal.strip() == '1'
        wires[wire] = signal
        i += 1

    gates = []

    i += 1
    while i < len(lines):
        tokens = lines[i].split()
        gates.append(Gate(tokens[1], tokens[0], tokens[2], tokens[4]))
        i += 1



    return part1(gates, wires), part2_sol(lines)
