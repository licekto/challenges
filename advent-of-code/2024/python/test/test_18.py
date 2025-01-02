from .utility import *
from advent_of_code.day_18 import *


@pytest.mark.parametrize('file_path, bound, n, expected_part1, expected_part2',
                     [
                      (['18', 'ref.in'], 6, 12, 22, (6, 1)),
                      (['18', 'user.in'], 70, 1024, 364, (52, 28))
                     ])
def test_18(get_data, file_path, bound, n, expected_part1, expected_part2):
    assert(solve(get_data(file_path), bound, n) == (expected_part1, expected_part2))

