import re


def parse(lines):
    order = {}

    i = 0
    for line in lines:
        i += 1
        if len(line) == 0:
            break
        s = line.split('|')
        l, r = int(s[0]), int(s[1])
        if l not in order:
            order[l] = set()
        order[l].add(r)

    updates = []
    for j in range(i, len(lines)):
        updates.append(list(map(int, lines[j].split(','))))

    return order, updates


def check_update(order, update):
    for i in range(1, len(update)):
        for j in range(i):
            if update[i] in order and update[j] in order[update[i]]:
                return False
    return True


def sort_update(order, update):
    for i in range(1, len(update)):
        for j in range(i):
            if update[i] in order and update[j] in order[update[i]]:
                update.insert(j, update.pop(i))
                break


def solve(lines):

    res1 = 0
    res2 = 0

    order, updates = parse(lines)

    for update in updates:
        if check_update(order, update):
            res1 += update[int(len(update) / 2)]
        else:
            sort_update(order, update)
            res2 += update[int(len(update) / 2)]

    return res1, res2

