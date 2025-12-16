from .utility import *
from advent_of_code.day_12 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['12', 'ref.in'], 0, 0),
                          (['12', 'user.in'], 0, 0)
                         ])
def test_12(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
