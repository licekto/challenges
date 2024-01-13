from .utility import *
from advent_of_code.day_02 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['02', 'ref.in'], 8, 2286),
                          (['02', 'user.in'], 2476, 54911)])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))
