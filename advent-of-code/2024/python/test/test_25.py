from .utility import *
from advent_of_code.day_25 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['25', 'ref.in'], 0, 0),
                      (['25', 'user.in'], 0, 0)
                     ])
def test_25(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

