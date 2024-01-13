from .utility import *
from advent_of_code.day_18 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['18', 'ref.in'], 62, 952408144115),
                          (['18', 'user.in'], 38188, 93325849869340)
                          ])
def test_18(get_data, file_path, expected_part1, expected_part2):
    assert (solve(*parse(get_data(file_path))) == (expected_part1, expected_part2))
