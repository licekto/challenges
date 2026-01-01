from .utility import *
from advent_of_code.day_03 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['03', 'ref1.in'], 357, 3121910778619),
                          (['03', 'user.in'], 16812, 0)
                         ])
def test_03(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))

