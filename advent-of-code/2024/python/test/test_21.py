from .utility import *
from advent_of_code.day_21 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['21', 'ref.in'], 126384, 154115708116294),
                      (['21', 'user.in'], 184180, 231309103124520)
                     ])
def test_21(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

