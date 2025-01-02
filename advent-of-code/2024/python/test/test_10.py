from .utility import *
from advent_of_code.day_10 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['10', 'ref.in'], 36, 81),
                      (['10', 'user.in'], 820, 1786)
                     ])
def test_10(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

