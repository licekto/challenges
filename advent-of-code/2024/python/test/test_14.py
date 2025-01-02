from .utility import *
from advent_of_code.day_14 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2, w, h',
                     [(['14', 'ref.in'], 12, 45, 11, 7), # dummy result for part 2
                      (['14', 'user.in'], 221655456, 7858, 101, 103)
                     ])
def test_14(get_data, file_path, expected_part1, expected_part2, w, h):
    assert(solve(get_data(file_path), w, h) == (expected_part1, expected_part2))

