
def solution(map):
    m = len(map)
    n = len(map[0])

    Max = 20**20

    dist = [[Max] * n for i in range(m)]
    dist[0][0] = 1
    wall = [[Max] * n for i in range(m)]
    wall[0][0] = 1
    dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    q = []
    q.append((0, 0, False))

    min_dst = Max

    while len(q) != 0:
        cr, cc, cw = q.pop(0)

        for r, c in dirs:
            nr = cr + r
            nc = cc + c

            if nr < 0 or nr >= m or nc < 0 or nc >= n:
                continue

            nd = 0
            if cw is True:
                if map[nr][nc] == 1:
                    continue
                nd = wall[cr][cc] + 1
                if wall[nr][nc] <= nd:
                    continue
                wall[nr][nc] = nd
            else:
                nd = dist[cr][cc] + 1
                if map[nr][nc] == 1:
                    if wall[nr][nc] <= nd:
                        continue
                    wall[nr][nc] = nd
                else:
                    if dist[nr][nc] <= nd:
                        continue
                    dist[nr][nc] = nd

            if nr == m - 1 and nc == n - 1:
                min_dst = min(min_dst, nd)

            q.append((nr, nc, cw or map[nr][nc] == 1))

    return -1 if min_dst == Max else min_dst
