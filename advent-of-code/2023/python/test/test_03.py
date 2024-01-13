from .utility import *
from advent_of_code.day_03 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['03', 'ref.in'], 4361, 467835),
                          (['03', 'user.in'], 560670, 91622824)])
def test_03(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))
