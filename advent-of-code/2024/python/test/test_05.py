from .utility import *
from advent_of_code.day_05 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['05', 'ref.in'], 143, 123),
                      (['05', 'user.in'], 5064, 5152)
                     ])
def test_05(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

