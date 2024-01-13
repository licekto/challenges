from .utility import *
from advent_of_code.day_21 import *


@pytest.mark.parametrize('file_path, length, expected',
                         [(['21', 'ref.in'], 6, 16),
                          (['21', 'ref.in'], 64, 42),
                          (['21', 'user.in'], 64, 3642)
                          ])
def test_21_part1(get_data, file_path, length, expected):
    assert (solve_p1(get_data(file_path), length) == expected)


def test_21_part2(get_data):
    assert (solve_p2(get_data(['21', 'user.in'])) == 608603023105276)
