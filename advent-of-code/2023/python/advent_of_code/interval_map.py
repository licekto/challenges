class IntervalMapIterator:
    def __init__(self, head):
        self.__current = head

    def __iter__(self):
        return self

    def __next__(self):
        if self.__current is None:
            raise StopIteration
        value = self.__current.value
        self.__current = self.__current.next
        return value

    def __repr__(self):
        if self.__current is None:
            raise StopIteration
        return f'{self.__current.key}: {self.__current.value}'

    def __str__(self):
        return self.__repr__()

    def get(self):
        if self.__current is None:
            raise StopIteration
        return self.__current.key, self.__current.value


class Node:
    def __init__(self, key=None, value=None):
        self.key = key
        self.value = value
        self.next = None


class IntervalMap:

    def __init__(self):
        self.__begin = None
        self.__length = 0

    def __len__(self):
        return self.__length

    def get_interval_node(self, key):
        it = self.__begin

        prev = None
        while it is not None and key >= it.key:
            prev = it
            it = it.next

        return None if prev is None else prev

    def get_interval(self, key):
        n = self.get_interval_node(key)
        return None if n is None else (n.key, n.value)

    def __getitem__(self, key):
        it = self.__begin

        while it is not None and it.key != key:
            it = it.next

        return None if it is None else it.value

    def __setitem__(self, key, value):
        if self.__length == 0:
            self.__begin = Node(key, value)
            self.__length += 1
            return

        it = self.__begin
        prev = None

        while it is not None and key > it.key:
            prev = it
            it = it.next

        if it is None:
            prev.next = Node(key, value)
            self.__length += 1
            return

        if key == it.key:
            it.value = value
            return

        new_node = Node(key, value)

        if it == self.__begin:
            new_node.next = self.__begin
            self.__begin = new_node
        else:
            new_node.next = it
            prev.next = new_node

        self.__length += 1

    def __contains__(self, key):
        return self.__getitem__(key) is not None

    def __iter__(self):
        return IntervalMapIterator(self.__begin)

    def __repr__(self):
        result = 'IntervalMap: {'

        first = True
        for k, v in self.items():
            if not first:
                result += ', '
            first = False
            result += f'{k}: {repr(v)}'
        result += '}'

        return result

    def __str__(self):
        return self.__repr__()

    def items(self):
        it = self.__begin
        while it is not None:
            yield it.key, it.value
            it = it.next

    def to_list(self):
        return [(k, v) for k, v in self.items()]
