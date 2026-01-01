from .utility import *
from advent_of_code.day_08 import *


@pytest.mark.parametrize('file_path, size, expected1, expected2',
                         [(['08', 'ref1.in'], 10, 40, 25272),
                          (['08', 'user.in'], 1000, 26400, 8199963486)
                         ])
def test_08(get_data, file_path, size, expected1, expected2):
    assert(solve(get_data(file_path), size) == (expected1, expected2))
