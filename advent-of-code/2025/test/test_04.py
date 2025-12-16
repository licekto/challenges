from .utility import *
from advent_of_code.day_04 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['04', 'ref.in'], 13, 43),
                          (['04', 'user.in'], 1516, 0)
                         ])
def test_04(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
