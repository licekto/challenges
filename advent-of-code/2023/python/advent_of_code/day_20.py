from collections import deque
import math


ON = 'on'
OFF = 'off'
LOW = 'low'
HIGH = 'high'
BROADCASTER_STR = 'broadcaster'


class Broadcaster:
    def __init__(self, nodes):
        self.name = 'broadcaster'
        self.nodes = nodes

    def exec(self, src, pulse):
        return [(pulse, dst, self.name) for dst in self.nodes]

    def print(self):
        print(f'Broadcaster: {self.name} -> {self.nodes}')


class FlipFlop:
    def __init__(self, name, nodes):
        self.name = name
        self.nodes = nodes
        self.state = OFF

    def flip(self):
        self.state = ON if self.state == OFF else OFF

    def exec(self, src, pulse):
        if pulse == HIGH:
            return []

        new_pulse = HIGH if self.state == OFF else LOW
        self.flip()

        return [(new_pulse, dst, self.name) for dst in self.nodes]

    def print(self):
        print(f'FlipFlop: {self.name}, state={self.state} -> {self.nodes}')


class Conjunction:
    def __init__(self, name, nodes):
        self.name = name
        self.nodes = nodes
        self.memory = {}

    def add_input(self, node_input):
        self.memory[node_input] = LOW

    def exec(self, src, pulse):
        self.memory[src] = pulse
        new_pulse = HIGH
        if all([pulse == HIGH for pulse in self.memory.values()]):
            new_pulse = LOW
        return [(new_pulse, dst, self.name) for dst in self.nodes]

    def print(self):
        print(f'Conjunction: {self.name}, memory={self.memory} -> {self.nodes}')


def parse(lines):
    system = {}
    inputs = {}

    for line in lines:
        src = line.split(' ')[0][1:]
        dst = [x.strip() for x in line.split(' -> ')[1].split(',')]

        if line[0] == '%':
            system[src] = FlipFlop(src, dst)
        elif line[0] == '&':
            system[src] = Conjunction(src, dst)
        elif line[0:len(BROADCASTER_STR)] == BROADCASTER_STR:
            system[BROADCASTER_STR] = Broadcaster(dst)
            src = BROADCASTER_STR
        else:
            raise Exception(f'Unknown type: {line}')

        for d in dst:
            if d not in inputs:
                inputs[d] = [src]
            else:
                inputs[d].append(src)

    for name, inputs in inputs.items():
        if name not in system:
            continue
        if isinstance(system[name], Conjunction):
            for i in inputs:
                system[name].add_input(i)

    return system


def print_system(system):
    for name, data in system.items():
        data.print()


def push_button1(system):
    q = deque()

    q.append((LOW, BROADCASTER_STR, 'init'))
    low_pulse_count, high_pulse_count = 0, 0

    while len(q) != 0:
        pulse, node, src = q.popleft()
        low_pulse_count += 1 if pulse == LOW else 0
        high_pulse_count += 1 if pulse == HIGH else 0
        if node not in system:
            continue
        for n in system[node].exec(src, pulse):
            q.append(n)

    return low_pulse_count, high_pulse_count


def solve_p1(system):
    count = 1000
    low_pulse_count, high_pulse_count = 0, 0

    for i in range(count):
        lo, hi = push_button1(system)
        low_pulse_count += lo
        high_pulse_count += hi
    return low_pulse_count * high_pulse_count


def push_button2(system, state):
    q = deque()

    q.append((LOW, BROADCASTER_STR, 'init'))

    while len(q) != 0:
        pulse, node, src = q.popleft()
        if node in state and pulse == LOW:
            state.remove(node)
            return True
        if node not in system:
            continue
        for n in system[node].exec(src, pulse):
            q.append(n)
    return False


def solve_p2(system):
    count = 1

    state = {'fh', 'mf', 'fz', 'ss'}
    counts = []
    while True:
        if push_button2(system, state):
            counts.append(count)
        count += 1
        if len(state) == 0:
            break

    return math.lcm(*counts)
