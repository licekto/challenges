import copy
from collections import deque
import z3

class Machine:
    def __init__(self, indicator, buttons, joltage):
        self.indicator = indicator
        self.buttons = buttons
        self.joltage = joltage

    def __str__(self):
        return f'[{''.join(self.indicator)}], b={self.buttons}, j={self.joltage}'

    def init_state(self):
        return ['.'] * len(self.indicator)

    def press_button(self, b, state):
        button = self.buttons[b]
        new_state = copy.deepcopy(state)

        for i in button:
            new_state[i] = '#' if new_state[i] == '.' else '.'
        return new_state

    def matches(self, state):
        return self.indicator == state

    def get_buttons_len(self):
        return len(self.buttons)

    def get_buttons(self):
        return self.buttons

    def get_joltages(self):
        return self.joltage


def parse_machine(line):
    e = line.split(' ')
    indicator = [l for l in e[0][1:-1]]
    buttons = [list(map(int, b[1:-1].split(','))) for b in e[1:-1]]
    joltage = list(map(int, e[-1].strip()[1:-1].split(',')))
    return Machine(indicator, buttons, joltage)


def solve_machine(machine):
    q = deque()
    q.append((machine.init_state(), 0))

    seen_states = set()

    min_steps = float('inf')
    while q:
        cur_state, steps = q.popleft()
        if machine.matches(cur_state):
            min_steps = min(min_steps, steps)

        state_str = ''.join(list(map(str, cur_state)))
        if steps >= min_steps or state_str in seen_states:
            continue
        seen_states.add(state_str)

        for i in range(machine.get_buttons_len()):
            q.append((machine.press_button(i, cur_state), steps + 1))

    return min_steps


def solve_joltage(machine):
    bs = [z3.Int(f'b{i}') for i in range(machine.get_buttons_len())]

    opt = z3.Optimize()
    opt.add(
        [
            z3.Sum(bs[b] for b, button in enumerate(machine.get_buttons()) if j in button)
            == joltage
            for j, joltage in enumerate(machine.get_joltages())
        ]
    )
    opt.add([b >= 0 for b in bs])

    opt.minimize(z3.Sum(bs))
    assert opt.check() == z3.sat

    model = opt.model()

    return sum(model[b].as_long() for b in bs)


def solve(lines):

    machines = [parse_machine(line) for line in lines]
    r1 = sum(solve_machine(machine) for machine in machines)
    r2 = sum(solve_joltage(machine) for machine in machines)

    return r1, r2
