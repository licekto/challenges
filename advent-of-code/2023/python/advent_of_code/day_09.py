import re


def parse(lines):
    sequences = []
    for line in lines:
        sequences.append(list(map(int, re.findall(r'(-?\d+)', line))))
    return sequences


def get_next(sequence):
    new_sequence = []
    for i in range(1, len(sequence)):
        new_sequence.append(sequence[i] - sequence[i - 1])
    return new_sequence


def get_diff(sequences):
    front_diff, back_diff = 0, 0
    for seq in reversed(sequences):
        back_diff += seq[-1]
        front_diff = seq[0] - front_diff
    return front_diff, back_diff


def generate(sequence):
    generated = [sequence]
    while True:
        sequence = get_next(sequence)
        if all([x == 0 for x in sequence]):
            break
        generated.append(sequence)
    return generated


def solve(sequences):
    res1, res2 = 0, 0
    for sequence in sequences:
        f, b = get_diff(generate(sequence))
        res1 += b
        res2 += f
    return res1, res2
