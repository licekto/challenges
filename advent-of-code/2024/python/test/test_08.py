from .utility import *
from advent_of_code.day_08 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['08', 'ref.in'], 14, 34),
                      (['08', 'user.in'], 308, 1147)
                     ])
def test_08(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

