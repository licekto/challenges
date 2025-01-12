

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


    run = True
    while run:
        run = False
        for gate in gates:
            if gate.in1 in wires and gate.in2 in wires and gate.out not in wires:
                wires[gate.out] = gate.exec(wires[gate.in1], wires[gate.in2])
                run = True

    res1 = int(''.join([v for k, v in sorted([(k, '1' if v else '0') for k, v in wires.items() if k[0] == 'z'], reverse=True)]), 2)

    return res1, 0
