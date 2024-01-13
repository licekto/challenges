from .utility import *
from advent_of_code.day_20 import *


@pytest.mark.parametrize('file_path, expected',
                         [(['20', 'ref.in'], 32000000),
                          (['20', 'ref1.in'], 11687500),
                          (['20', 'user.in'], 787056720)
                          ])
def test_20_part1(get_data, file_path, expected):
    assert (solve_p1(parse(get_data(file_path))) == expected)


def test_20_part2(get_data):
    assert(solve_p2(parse(get_data(['20', 'user.in']))) == 212986464842911)
