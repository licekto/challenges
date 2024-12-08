from .utility import *
from advent_of_code.day_07 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['07', 'ref.in'], 3749, 11387),
#                      (['07', 'user.in'], 1298300076754, 248427118972289)
                     ])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

