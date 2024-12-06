DIRS = '>v<^'

class Map:
    def __init__(self, data):
        self.__data = data


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
    
    
    def print_m(self, sx, sy, d):
        print('-----------')
        for y in range(len(self.__data)):
            for x in range(len(self.__data[0])):
                if sx == x and sy == y:
                    print(DIRS[d], end='')
                else:
                    print(self.__data[y][x], end='')
            print()
    
    def get_next(self, x, y, d):
        nexts = {'>': (0, 1), 'v': (1, 0), '<': (0, -1), '^': (-1, 0)}
        return x + nexts[DIRS[d]][1], y + nexts[DIRS[d]][0]

    def is_on_map(self, x, y):
        return x >= 0 and x < len(self.__data[0]) and y >= 0 and y < len(self.__data)

    def at(self, x, y):
        return self.__data[y][x]



def is_loop(m, x, y, d):
    visited = set()
    visited.add((x, y, d))

    while True:
        pass

def solve(lines):

    res1 = 0
    res2 = 0

    m = Map(lines)

    x, y, d = m.find_start()

    visited = set()
    visited.add((x, y))
    while True:
        nx, ny = m.get_next(x, y, d)
        if not m.is_on_map(nx, ny):
            break

        if m.at(nx, ny) == '#':
            d = (d + 1) % len(DIRS)
            nx, ny = m.get_next(x, y, d)

        x = nx
        y = ny
        visited.add((x, y))


    res1 = len(visited)

    return res1, res2

