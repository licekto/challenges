
class Map:

    SPLITTER = '^'

    def __init__(self, lines):
        self.__map = [list(row) for row in lines]
        self.__start = Map.__find_start(self.__map)
        self.__split_cnt = 0
        self.__coord_tl = {}


    @staticmethod
    def __find_start(m):
        for y in range(0, len(m)):
            for x in range(0, len(m[0])):
                if m[y][x] == 'S':
                    return x, y
        return None


    def get_start(self):
        return self.__start


    def __is_valid(self, x, y):
        return 0 <= x < len(self.__map[0]) and 0 <= y < len(self.__map)


    def step(self, beam):
        x, y = beam
        y += 1
        if not self.__is_valid(x, y):
            return None

        if self.__map[y][x] == Map.SPLITTER:
            result = []
            for i in [-1, 1]:
                nx, ny = x + i, y
                if self.__is_valid(nx, ny):
                    result.append((nx, ny))
            self.__split_cnt += 1 if len(result) == 2 else 0
            return result
        return [(x, y)]


    def get_timelines(self, beam):
        if beam in self.__coord_tl:
            return self.__coord_tl[beam]

        new_beams = self.step(beam)

        if new_beams is None:
            return 1

        cnt = 0
        for new_beam in new_beams:
            cnt += self.get_timelines(new_beam)

        self.__coord_tl[beam] = cnt
        return cnt


    def get_split_cnt(self):
        return self.__split_cnt

def solve(lines):

    m = Map(lines)

    r2 = m.get_timelines(m.get_start())
    r1 = m.get_split_cnt()

    return r1, r2
