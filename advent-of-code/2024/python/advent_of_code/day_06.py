DIRS = '>v<^'

def get_next_dir(d):
    return (d + 1) % len(DIRS)

class Map:
    def __init__(self, data):
        self.__data = data
        self.__ox = -1
        self.__oy = -1

    def find_start(self):
        for y in range(len(self.__data)):
            for x in range(len(self.__data[0])):
                if self.__data[y][x] in '<>v^':
                    d = self.__data[y][x]
                    for i in range(len(DIRS)):
                        if d == DIRS[i]:
                            d = i                   
                    return x, y, d
        return -1, -1, 0
    
    def get_next(self, x, y, d):
        nexts = {'>': (0, 1), 'v': (1, 0), '<': (0, -1), '^': (-1, 0)}
        return x + nexts[DIRS[d]][1], y + nexts[DIRS[d]][0]

    def is_on_map(self, x, y):
        return x >= 0 and x < len(self.__data[0]) and y >= 0 and y < len(self.__data)

    def at(self, x, y):
        return self.__data[y][x]

    def set_obst(self, x, y):
        self.__ox = x
        self.__oy = y

    def unset_obst(self, x, y):
        self.__ox = -1
        self.__oy = -1

    def is_obst(self, x, y):
        return self.__data[y][x] == '#' or (x == self.__ox and y == self.__oy)


def is_loop(m, x, y, d, obstructions):
    visited = set()
    ret = False

    while True:
        if (x, y, d) in visited:
            return True
        visited.add((x, y, d))

        nx, ny = m.get_next(x, y, d)
        
        if not m.is_on_map(nx, ny):
            break

        while m.is_obst(nx, ny):
            d = get_next_dir(d)
            nx, ny = m.get_next(x, y, d)

        x = nx
        y = ny
        if not m.is_on_map(nx, ny):
            break

    return False

def solve(lines):

    res1 = 0
    res2 = 0

    m = Map(lines)

    x, y, d = m.find_start()
    sx, sy, sd = x, y, d

    visited = set()
    visited.add((x, y))
    obstructions = set()

    while True:
        nx, ny = m.get_next(x, y, d)
        if not m.is_on_map(nx, ny):
            break

        while m.at(nx, ny) == '#':
            d = get_next_dir(d)
            nx, ny = m.get_next(x, y, d)

        x = nx
        y = ny

        visited.add((x, y))

    for y in range(len(lines)):
        for x in range(len(lines[0])):
            if lines[y][x] != '.':
                continue
            m.set_obst(x, y)
            if is_loop(m, sx, sy, sd, obstructions):
                res2 += 1
            m.unset_obst(x, y)


    res1 = len(visited)

    return res1, res2

