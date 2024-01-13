import math
import itertools


def get_ref_cnt(pos, dst, dim):
    return int(math.ceil(float(pos + dst) / dim)) - 1


def ref_axis(n, dim, pos):
    reflections = []
    for i in range(n + 1):
        ref = i * dim
        ref += dim - pos if i % 2 != 0 else pos
        reflections.append(ref)
        reflections.append(-ref)
    return reflections


def generate_reflections(ref_cnt, dim, pos):
    x_reflections = ref_axis(ref_cnt[0], dim[0], pos[0])
    y_reflections = ref_axis(ref_cnt[1], dim[1], pos[1])
    return set(itertools.product(x_reflections, y_reflections))


def dist(pos, ref):
    a = abs(pos[0] - ref[0])
    b = abs(pos[1] - ref[1])
    return a ** 2 + b ** 2


def compose(r_dict, reflections, pos, s, dst):
    for r in reflections:
        if dist(pos, r) > dst ** 2:
            continue
        a = math.atan2(pos[0] - r[0], pos[1] - r[1])
        if a not in r_dict or dist(pos, r_dict[a][0]) > dist(pos, r):
            r_dict[a] = (r, s)


def solution(dim, player, trainer, dst):
    player = tuple(player)
    xrc = get_ref_cnt(player[0], dst, dim[0])
    yrc = get_ref_cnt(player[1], dst, dim[1])

    p_refs = generate_reflections((xrc, yrc), dim, player)
    p_refs.remove(player)
    t_refs = generate_reflections((xrc, yrc), dim, trainer)

    r_dict = {}
    compose(r_dict, p_refs, player, 's', dst)
    compose(r_dict, t_refs, player, 't', dst)

    return len([s for _, s in r_dict.values() if s == 't'])
