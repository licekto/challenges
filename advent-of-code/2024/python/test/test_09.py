from .utility import *
from advent_of_code.day_09 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['09', 'ref.in'], 1928, 2858),
#                      (['09', 'user.in'], 6340197768906, 6363913128533)
                     ])
def test_09(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

