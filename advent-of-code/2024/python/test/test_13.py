from .utility import *
from advent_of_code.day_13 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['13', 'ref.in'], 480, 875318608908),
                      (['13', 'user.in'], 39996, 73267584326867)
                     ])
def test_13(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

