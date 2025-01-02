from .utility import *
from advent_of_code.day_04 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['04', 'ref.in'], 18, 9),
                      (['04', 'user.in'], 2618, 2011)
                     ])
def test_04(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

