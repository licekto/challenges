from .utility import *
from advent_of_code.day_22 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['22', 'ref.in'], 37990510, 23),
                      (['22', 'user.in'], 18317943467, 2018)
                     ])
def test_22(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

