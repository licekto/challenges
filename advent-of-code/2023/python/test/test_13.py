from .utility import *
from advent_of_code.day_13 import *


def test_reflections(get_data):
    assert (find_horizontal(parse(get_data(['13', 'map01.in']))[0]) == 4)
    assert (find_vertical(parse(get_data(['13', 'map01.in']))[0]) == 2)

    maps = parse(get_data(['13', 'ref.in']))

    assert (find_horizontal(maps[0]) == 0)
    assert (find_vertical(maps[0]) == 5)

    assert (find_horizontal(maps[1]) == 4)
    assert (find_vertical(maps[1]) == 0)

    m = parse(get_data(['13', 'map02.in']))[0]
    assert (find_horizontal(m, allowed_diff=1) == 11)
    assert (find_vertical(m, allowed_diff=1) == 0)


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['13', 'ref.in'], 405, 400),
                          (['13', 'user.in'], 34100, 33106)
                          ])
def test_13(get_data, file_path, expected_part1, expected_part2):
    assert (solve(parse(get_data(file_path))) == (expected_part1, expected_part2))
