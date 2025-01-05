from .utility import *
from advent_of_code.day_20 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['20', 'ref.in'], 0, 0),
                      (['20', 'user.in'], 1445, 1008040)
                     ])
def test_20(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

