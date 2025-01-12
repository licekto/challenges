from collections import defaultdict


def mix_and_prune(n, v):
    return (n ^ v) % 16777216


def evolve(n):
    n = mix_and_prune(n, n * 64)
    n = mix_and_prune(n, n // 32)
    n = mix_and_prune(n, n * 2048)
    return n


def resolve_seq(sequences):

    seq_dict = defaultdict(lambda: 0)

    for seq in sequences:
        visited = set()
        for i in range(4, len(seq)):
            sub_seq = (seq[i - 3][1], seq[i - 2][1], seq[i - 1][1], seq[i][1])
            if sub_seq not in visited:
                seq_dict[sub_seq] += seq[i][0]
                visited.add(sub_seq)

    return max(seq_dict.values())


def solve(lines):

    res1 = 0
    sequences = []
    for secret in [int(l) for l in lines]:
        seq = []
        prev = None
        for i in range(2000):
            price = secret % 10
            diff = None if prev is None else price - prev
            prev = price
            seq.append((price,  diff))
            secret = evolve(secret)
        sequences.append(seq)
        res1 += secret

    res2 = resolve_seq(sequences)

    return res1, res2
