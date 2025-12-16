from .utility import *
from advent_of_code.day_11 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['11', 'ref.in'], 0, 0),
                          (['11', 'user.in'], 0, 0)
                         ])
def test_11(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
