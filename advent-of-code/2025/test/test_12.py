from .utility import *
from advent_of_code.day_12 import *


@pytest.mark.parametrize('file_path, expected',
                         [(['12', 'ref.in'], 1), # should be 2
                          (['12', 'user.in'], 463)
                         ])
def test_12(get_data, file_path, expected):
    assert(solve(get_data(file_path)) == expected)
