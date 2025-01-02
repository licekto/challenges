from .utility import *
from advent_of_code.day_16 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['16', 'ref.in'], 7036, 45),
                      (['16', 'ref1.in'], 11048, 64),
                      (['16', 'user.in'], 99460, 500)
                     ])
def test_16(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

