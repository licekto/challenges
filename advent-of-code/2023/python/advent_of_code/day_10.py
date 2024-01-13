from .term_colors import *


def find_start(maze):
    for i in range(len(maze)):
        for j in range(len(maze[0])):
            if maze[i][j] == 'S':
                return i, j
    return None, None


go_up = ((-1, 0), ['|', '7', 'F'])
go_down = ((1, 0), ['|', 'J', 'L'])
go_left = ((0, -1), ['-', 'L', 'F'])
go_right = ((0, 1), ['-', 'J', '7'])

directions = [('|', [go_up, go_down]),
              ('-', [go_left, go_right]),
              ('J', [go_up, go_left]),
              ('L', [go_up, go_right]),
              ('7', [go_down, go_left]),
              ('F', [go_down, go_right])]


def get_next(maze, from_coord, to_coord):

    for current, current_dirs in directions:
        if maze[to_coord[0]][to_coord[1]] != current:
            continue
        for d, neighbors in current_dirs:
            ni, nj = to_coord[0] + d[0], to_coord[1] + d[1]
            if ni not in range(len(maze)) or nj not in range(len(maze[0])):
                continue
            if (ni, nj) == from_coord:
                continue
            if maze[ni][nj] not in neighbors:
                continue
            return ni, nj

    return None, None


def get_first_next(maze, i, j):
    directions_local = [go_up, go_down, go_left, go_right]
    for d, neighbors in directions_local:
        ni, nj = i + d[0], j + d[1]
        if ni not in range(len(maze)) or nj not in range(len(maze[0])):
            continue
        if maze[ni][nj] not in neighbors:
            continue
        return ni, nj
    return None, None


def print_maze(maze, path):
    print()
    for i in range(len(maze)):
        for j in range(len(maze[0])):
            if (i, j) in path:
                print(f'{Colors.RED}{maze[i][j]}{Colors.END}', end='')
            elif maze[i][j] == 'I':
                print(f'{Colors.GREEN}{maze[i][j]}{Colors.END}', end='')
            else:
                print(maze[i][j], end='')
        print()
    print('------------------')


def solve(maze):

    pi, pj = find_start(maze)
    i, j = get_first_next(maze, pi, pj)
    steps = 1
    path = set()
    path.add((pi, pj))
    path.add((i, j))

    while True:
        ni, nj = get_next(maze, (pi, pj), (i, j))
        pi, pj = i, j
        i, j = ni, nj
        steps += 1
        path.add((i, j))
        if i is None and j is None:
            break

    maze = list(map(list, maze))
    inside_count = 0
    for i in range(len(maze)):
        is_inside = False
        for j in range(len(maze[0])):
            if (i, j) in path and maze[i][j] in '|JL':
                is_inside = not is_inside
            elif (i, j) not in path:
                maze[i][j] = 'I' if is_inside else 'O'
                inside_count += 1 if is_inside else 0

    print_maze(maze, path)

    return steps // 2, inside_count
