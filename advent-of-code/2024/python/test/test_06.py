from .utility import *
from advent_of_code.day_06 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['06', 'ref.in'], 41, 0),
                      (['06', 'user.in'], 5531, 0)
                     ])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

