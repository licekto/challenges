from .utility import *
from advent_of_code.day_14 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['14', 'ref.in'], 136, 64),
                          (['14', 'user.in'], 109098, 100064)
                          ])
def test_14(get_data, file_path, expected_part1, expected_part2):
    assert (solve_part1(to_list(get_data(file_path))) == expected_part1)
    assert (solve_part2(to_list(get_data(file_path))) == expected_part2)
