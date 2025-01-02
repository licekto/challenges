from .utility import *
from advent_of_code.day_22 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['22', 'ref.in'], 0, 0),
                      (['22', 'user.in'], 0, 0)
                     ])
def test_22(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

