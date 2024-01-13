from .utility import *
from advent_of_code.day_10 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['10', 'ref_part1_1.in'], 4, 1),
                          (['10', 'ref_part1_2.in'], 8, 1),
                          (['10', 'ref_part2_1.in'], 23, 4),
                          (['10', 'ref_part2_2.in'], 22, 4),
                          (['10', 'ref_part2_3.in'], 70, 8),
                          (['10', 'ref_part2_4.in'], 80, 10),
                          (['10', 'user.in'], 7066, 401)
                          ])
def test_10(get_data, file_path, expected_part1, expected_part2):
    assert (solve(get_data(file_path)) == (expected_part1, expected_part2))

