from .utility import *
from advent_of_code.day_16 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['16', 'ref.in'], 46, 51),
                          (['16', 'user.in'], 7434, 8183)
                          ])
def test_16(get_data, file_path, expected_part1, expected_part2):
    assert (solve_p1(get_data(file_path)) == expected_part1)
    assert (solve_p2(get_data(file_path)) == expected_part2)

