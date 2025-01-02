from .utility import *
from advent_of_code.day_15 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [
                      (['15', 'ref2.in'], 908, 618),
                      (['15', 'ref.in'], 2028, 1751),
                      (['15', 'ref1.in'], 10092, 9021),
                      (['15', 'user.in'], 1406628, 1432781)
                     ])
def test_15(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

