from .utility import *
from advent_of_code.day_09 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['09', 'ref.in'], 114, 2),
                          (['09', 'user.in'], 1887980197, 990)
                          ])
def test_09(get_data, file_path, expected_part1, expected_part2):
    assert (solve(parse(get_data(file_path))) == (expected_part1, expected_part2))

