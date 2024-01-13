import functools, re


def is_in(m, k, v):
    return k in m and m[k] == v


class Evaluator:
    INIT_CHAR_MAP = {'A': 0, 'K': 0, 'Q': 0, 'J': 0, 'T': 0, '9': 0,
                     '8': 0, '7': 0, '6': 0, '5': 0, '4': 0, '3': 0, '2': 0}

    def __init__(self):
        self.__char_map = self.INIT_CHAR_MAP

    @staticmethod
    def __freq_analysis(data):
        f = {}
        for v in data:
            f[v] = 1 if v not in f else f[v] + 1
        return f

    def __of_a_kind(self, n):
        return any(v == n for c, v in self.__char_map.items())

    def __pairs_analysis(self):
        return self.__freq_analysis([x == 2 or x == 1 for x in self.__char_map])

    def __five_of_a_kind(self):
        return self.__of_a_kind(5)

    def __four_of_a_kind(self):
        return self.__of_a_kind(4)

    def __full_house(self):
        f = self.__freq_analysis(self.__char_map.values())
        return 2 in f and 3 in f and f[2] == 1 and f[3] == 1

    def __three_of_a_kind(self):
        f = self.__freq_analysis(self.__char_map.values())
        return is_in(f, 1, 2) and is_in(f, 3, 1)

    def __two_pair(self):
        f = self.__freq_analysis(self.__char_map.values())
        return is_in(f, 1, 1) and is_in(f, 2, 2)

    def __one_pair(self):
        f = self.__freq_analysis(self.__char_map.values())
        return is_in(f, 1, 3) and is_in(f, 2, 1)

    def __high_card(self):
        f = self.__freq_analysis(self.__char_map.values())
        return is_in(f, 1, 5)

    def get_type_raw(self, hand):
        self.__char_map = self.__freq_analysis(hand)

        cb_list = [(self.__five_of_a_kind, 7), (self.__four_of_a_kind, 6), (self.__full_house, 5),
                   (self.__three_of_a_kind, 4), (self.__two_pair, 3), (self.__one_pair, 2), (self.__high_card, 1)]

        for cb, rank in cb_list:
            if cb():
                return rank
        return 0

    def get_type(self, hand, wildcard=False):
        t = self.get_type_raw(hand)

        if not wildcard or 'J' not in hand:
            return t

        max_rank = 0
        for c in self.INIT_CHAR_MAP.keys():
            h = hand.replace('J', c)
            max_rank = max(max_rank, self.get_type_raw(h))
        return max_rank


def compare_first_rule(h1, h2, wildcard=False):
    e = Evaluator()
    t1, t2 = e.get_type(h1, wildcard), e.get_type(h2, wildcard)

    if t1 < t2:
        return -1
    elif t1 == t2:
        return 0
    return 1


def compare_second_rule(h1, h2, wildcard=False):
    rank = {'A': 14, 'K': 13, 'Q': 12, 'J': 11, 'T': 10, '9': 9, '8': 8, '7': 7, '6': 6, '5': 5, '4': 4, '3': 3, '2': 2}
    if wildcard:
        rank['J'] = 1
    for i in range(5):
        if rank[h1[i]] < rank[h2[i]]:
            return -1
        elif rank[h1[i]] > rank[h2[i]]:
            return 1
    return 0


def compare_hands(h1, h2, wildcard=False):
    r = compare_first_rule(h1, h2, wildcard)
    if r != 0:
        return r
    return compare_second_rule(h1, h2, wildcard)


def compare(b1, b2):
    return compare_hands(b1[0], b2[0])


def compare_wildcard(b1, b2):
    return compare_hands(b1[0], b2[0], True)


def calculate(bids, wildcard):
    bids.sort(key=functools.cmp_to_key(compare_wildcard if wildcard else compare))
    res = 0
    for r, b in enumerate(bids):
        res += (r + 1) * b[1]
    return res


def solve(lines):
    bids = []
    for line in lines:
        m = re.match(r'([A-Z0-9]+) (\d+)', line)
        bids.append((m.group(1), int(m.group(2))))

    return calculate(bids, wildcard=False), calculate(bids, wildcard=True)
