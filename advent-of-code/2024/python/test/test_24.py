from .utility import *
from advent_of_code.day_24 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['24', 'ref.in'], 0, 0),
                      (['24', 'user.in'], 0, 0)
                     ])
def test_24(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

