import heapq


class Item:
    def __init__(self, heat, coord, direction, straight_count):
        self.heat = heat
        self.coord = coord
        self.direction = direction
        self.straight_count = straight_count

    def __lt__(self, other):
        return self.heat < other.heat


class Solver:
    MAX = 10 ** 18

    def __init__(self, puzzle):
        self.__puzzle = puzzle
        self.__end = (len(puzzle) - 1, len(puzzle[0]) - 1)

    @staticmethod
    def __get_next(coord, direction, options):
        for current_direction, new_direction, new_i, new_j in options:
            if direction == current_direction:
                return coord[0] + new_i, coord[1] + new_j, new_direction
        raise Exception(f'Unknown direction={direction}')

    @staticmethod
    def __get_next_straight(coord, direction):
        return Solver.__get_next(coord, direction, [('>', '>', 0, 1), ('v', 'v', 1, 0), ('<', '<', 0, -1), ('^', '^', -1, 0)])

    @staticmethod
    def __get_next_left(coord, direction):
        return Solver.__get_next(coord, direction, [('>', '^', -1, 0), ('v', '>', 0, 1), ('<', 'v', 1, 0), ('^', '<', 0, -1)])

    @staticmethod
    def __get_next_right(coord, direction):
        return Solver.__get_next(coord, direction, [('>', 'v', 1, 0), ('v', '<', 0, -1), ('<', '^', -1, 0), ('^', '>', 0, 1)])

    def __is_valid(self, coord):
        return 0 <= coord[0] < len(self.__puzzle) and 0 <= coord[1] < len(self.__puzzle[0])

    def __heat(self, coord):
        return int(self.__puzzle[coord[0]][coord[1]])

    def dijkstra(self, cond1, cond2):
        pq = []
        closed_set = set()
        dist = {}

        for c, d in [((0, 1), '>'), ((1, 0), 'v')]:
            heat = self.__heat(c)
            pq.append(Item(heat, c, d, 2))
            closed_set.add((c[0], c[1], d, 2))
            dist[(c[0], c[1], d, 2)] = heat
        heapq.heapify(pq)

        while len(pq) != 0:
            current = heapq.heappop(pq)
            coord = (current.coord[0], current.coord[1])
            closed_set.add((coord[0], coord[1], current.direction, current.straight_count))

            if coord == self.__end:
                return current.heat

            d = current.direction

            neighbors = []
            if cond1(current.straight_count):
                neighbors.append(Solver.__get_next_straight(coord, d))
            if cond2(current.straight_count):
                neighbors.append(Solver.__get_next_right(coord, d))
                neighbors.append(Solver.__get_next_left(coord, d))

            for ni, nj, nd in neighbors:
                neigh_coord = (ni, nj)
                if not self.__is_valid(neigh_coord):
                    continue

                neigh_heat = self.__heat(neigh_coord)
                new_heat = current.heat + neigh_heat

                neigh_straight_cnt = current.straight_count + 1 if d == nd else 1
                new_elem = (ni, nj, nd, neigh_straight_cnt)
                if new_elem not in closed_set and (new_elem not in dist or dist[new_elem] < new_heat):
                    dist[new_elem] = new_heat
                    heapq.heappush(pq, Item(new_heat, neigh_coord, nd, neigh_straight_cnt))

        return None


def solve_p1(puzzle):
    solver = Solver(puzzle)
    return solver.dijkstra(lambda x: x < 3, lambda x: True)


def solve_p2(puzzle):
    solver = Solver(puzzle)
    return solver.dijkstra(lambda x: x < 10, lambda x: x >= 4)
