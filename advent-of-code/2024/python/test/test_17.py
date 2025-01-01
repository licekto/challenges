from .utility import *
from advent_of_code.day_17 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [
                      (['17', 'ref.in'], '4,6,3,5,6,3,5,2,1,0', 105734774294938),
                      (['17', 'user.in'], '7,3,5,7,5,7,4,3,0', 105734774294938)
                     ])
def test_02(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

