from .utility import *
from advent_of_code.day_21 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['21', 'ref.in'], 0, 0),
                      (['21', 'user.in'], 0, 0)
                     ])
def test_21(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

