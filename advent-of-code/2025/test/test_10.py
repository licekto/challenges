from .utility import *
from advent_of_code.day_10 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['10', 'ref1.in'], 7, 33),
                          (['10', 'user.in'], 479, 19574)
                         ])
def test_10(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
