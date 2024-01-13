from collections import deque


def parse(lines):
    bricks_map = {}
    bricks_set = set()

    for line in lines:
        s = line.split('~')
        brick = Brick(Coord.from_str(s[0]), Coord.from_str(s[1]))
        bricks_set.add(brick)
        for z in range(brick.begin.z, brick.end.z + 1):
            if z not in bricks_map:
                bricks_map[z] = set()
            bricks_map[z].add(brick)

    return dict(sorted(bricks_map.items())), bricks_set


class Coord:
    @staticmethod
    def from_str(coord_str):
        arr = [int(x) for x in coord_str.split(',')]
        return Coord(arr[0], arr[1], arr[2])

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z


def ov(a1, a2, b1, b2):
    return a1 <= b1 <= a2 or a1 <= b2 <= a2


class Brick:
    def __init__(self, begin, end):
        self.begin = begin if begin.z <= end.z else end
        self.end = end if begin.z <= end.z else begin
        self.bricks_supported = set()
        self.supported_by = set()

    def collides_with(self, other):
        return (ov(self.begin.x, self.end.x, other.begin.x, other.end.x) or ov(other.begin.x, other.end.x, self.begin.x, self.end.x))\
                and (ov(self.begin.y, self.end.y, other.begin.y, other.end.y) or ov(other.begin.y, other.end.y, self.begin.y, self.end.y))

    def can_be_removed(self):
        for supported in self.bricks_supported:
            if len(supported.supported_by) == 1:
                return False
        return True

    def __str__(self):
        return f'B[{self.begin.x}, {self.begin.y}, {self.begin.z}], [{self.end.x}, {self.end.y}, {self.end.z}]B'

    def __repr__(self):
        return str(self)

    def __eq__(self, other):
        return self.begin == other.begin and self.end == other.end

    def __hash__(self):
        return hash((self.begin.x, self.begin.y, self.begin.z, self.end.x, self.end.y, self.end.z))

    def move(self, bricks_map, bricks_set, new_z):
        if self.begin.z == new_z:
            return False

        for z in range(self.begin.z, self.end.z + 1):
            bricks_map[z].remove(self)
            if len(bricks_map[z]) == 0:
                del (bricks_map[z])
        bricks_set.remove(self)

        orig_z_len = self.end.z - self.begin.z
        self.begin.z = new_z
        self.end.z = new_z + orig_z_len

        for z in range(self.begin.z, self.end.z + 1):
            if z not in bricks_map:
                bricks_map[z] = set()
            bricks_map[z].add(self)
        bricks_set.add(self)

        return True


def settle(bricks_map, bricks_set, brick):
    collisions = set()
    last_z = -1

    for z in range(brick.begin.z - 1, 0, -1):
        if z not in bricks_map or len(bricks_map[z]) == 0:
            continue
        for other_brick in bricks_map[z]:
            if brick.collides_with(other_brick):
                collisions.add(other_brick)
        if len(collisions) != 0:
            last_z = z
            break

    if len(collisions) == 0:
        last_z = 0
    last_z += 1

    moved = brick.move(bricks_map, bricks_set, last_z)

    for collision in collisions:
        collision.bricks_supported.add(brick)
        brick.supported_by.add(collision)

    return moved


def settle_all(bricks_map, bricks_set):
    visited = set()
    z = 1

    moved_count = 0
    while len(visited) != len(bricks_set):
        if z in bricks_map:
            for brick in list(bricks_map[z]):
                if settle(bricks_map, bricks_set, brick):
                    moved_count += 1
                visited.add(brick)
        z += 1

    return moved_count


def bfs(brick):
    q = deque()
    for s in brick.bricks_supported:
        q.append(s)
    fallen = set()
    fallen.add(brick)

    while q:
        b = q.popleft()

        support = len(b.supported_by)
        for s in b.supported_by:
            if s in fallen:
                support -= 1

        if support == 0:
            fallen.add(b)
            for s in b.bricks_supported:
                q.append(s)

    return len(fallen) - 1


def solve(bricks_map, bricks_set):
    settle_all(bricks_map, bricks_set)

    to_remove = set()
    cant_remove = set()
    for brick in bricks_set:
        if brick.can_be_removed():
            to_remove.add(brick)
        else:
            cant_remove.add(brick)

    cnt = 0
    for brick in cant_remove:
        cnt += bfs(brick)

    return len(to_remove), cnt
