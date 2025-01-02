from .utility import *
from advent_of_code.day_19 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['19', 'ref.in'], 6, 16),
                      (['19', 'user.in'], 228, 584553405070389)
                     ])
def test_19(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

