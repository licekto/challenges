

def parse(lines):
    return lines[0].split(',')


def hash_algo(s):
    current_value = 0
    for c in s:
        current_value += ord(c)
        current_value *= 17
        current_value %= 256
    return current_value


def solve_part1(steps):
    res = 0
    for s in steps:
        res += hash_algo(s)
    return res


class Boxes:

    class Hasher:
        def __init__(self):
            self.__cache = {}

        def get_hash(self, label):
            if label in self.__cache:
                return self.__cache[label]
            h = hash_algo(label)
            self.__cache[label] = h
            return h

    class Box:
        class Node:
            def __init__(self, label, length):
                self.label = label
                self.length = length
                self.prev = None
                self.next = None

        def __init__(self):
            self.__label_map = {}
            self.__head = None
            self.__tail = None

        def add(self, label, length):
            if label in self.__label_map:
                self.__label_map[label].length = length
                return

            new_node = self.Node(label, length)
            self.__label_map[label] = new_node

            if self.__tail is None:
                self.__head = self.__tail = new_node
            else:
                new_node.prev = self.__tail
                self.__tail.next = new_node
                self.__tail = new_node

        def remove(self, label):
            if label not in self.__label_map:
                return

            old_node = self.__label_map[label]

            if old_node == self.__head:
                if old_node == self.__tail:
                    self.__head = None
                    self.__tail = None

                self.__head = old_node.next
                if self.__head is not None:
                    self.__head.prev = None

            elif old_node == self.__tail:
                self.__tail = self.__tail.prev
                self.__tail.next = None

            else:
                old_node.prev.next = old_node.next
                old_node.next.prev = old_node.prev

            del(self.__label_map[label])

        def empty(self):
            return self.__head is None

        def print(self):
            it = self.__head
            while it is not None:
                print(f' [{it.label} {it.length}]', end='')
                it = it.next

        def get_result(self, idx):
            slot = 1
            result = 0
            it = self.__head
            while it is not None:
                result += idx * slot * it.length
                it = it.next
                slot += 1
            return result

    def __init__(self):
        self.__boxes = [self.Box() for i in range(256)]
        self.__hasher = self.Hasher()

    def add(self, label, length):
        h = self.__hasher.get_hash(label)
        self.__boxes[h].add(label, length)

    def remove(self, label):
        h = self.__hasher.get_hash(label)
        self.__boxes[h].remove(label)

    def print(self):
        for i, box in enumerate(self.__boxes):
            if box.empty():
                continue
            print(f'Box {i}:', end='')
            box.print()
            print()

    def get_result(self):
        result = 0
        for i, box in enumerate(self.__boxes):
            result += box.get_result(i + 1)
        return result


def solve_part2(steps):
    boxes = Boxes()

    for step in steps:
        action = '=' if '=' in step else '-'
        s = step.split(action)
        label = s[0]
        length = int(s[1]) if len(s[1]) > 0 else None
        if action == '=':
            boxes.add(label, length)
        else:
            boxes.remove(label)

    return boxes.get_result()
