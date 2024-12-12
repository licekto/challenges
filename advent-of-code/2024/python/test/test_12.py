from .utility import *
from advent_of_code.day_12 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['12', 'ref.in'], 1930, 1206),
                      (['12', 'user.in'], 1450422, 906606)
                     ])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

