
class IntervalMap:

    def __init__(self):
        self.__map = {}


    def __setitem__(self, l, h):
        if len(self.__map) == 0:
            self.__map[l] = h
            return

        new_start = l
        new_end = h
        keys_to_remove = []

        for k, v in self.__map.items():
            if k <= h + 1 and v >= l - 1:
                new_start = min(new_start, k)
                new_end = max(new_end, v)
                keys_to_remove.append(k)
        for k in keys_to_remove:
            del self.__map[k]

        self.__map[new_start] = new_end


    def is_in(self, v):
        for l, h in self.__map.items():
            if l <= v <= h:
                return True
        return False


    def get_intervals_length(self):
        return sum([v - k + 1 for k, v in self.__map.items()])


def solve(lines):
    r1, r2 = 0, 0

    interval_map = IntervalMap()

    i = 0
    while len(lines[i]):
        l, h = map(int, tuple(lines[i].split('-')))
        interval_map[l] = h
        i += 1

    i += 1

    while i < len(lines):
        iid = int(lines[i])
        if interval_map.is_in(iid):
            r1 += 1
        i += 1

    r2 = interval_map.get_intervals_length()

    return r1, r2

