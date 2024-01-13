from .utility import *
from advent_of_code.day_15 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['15', 'ref.in'], 1320, 145),
                          (['15', 'user.in'], 509784, 230197)
                          ])
def test_15(get_data, file_path, expected_part1, expected_part2):
    assert (solve_part1(parse(get_data(file_path))) == expected_part1)
    assert (solve_part2(parse(get_data(file_path))) == expected_part2)
