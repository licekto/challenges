from .utility import *
from advent_of_code.day_11 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['11', 'ref.in'], 55312, 65601038650482),
                      (['11', 'user.in'], 229043, 272673043446478)
                     ])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

