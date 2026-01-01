from .utility import *
from advent_of_code.day_09 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['09', 'ref1.in'], 50, 24),
                          (['09', 'user.in'], 4737096935, 1644094530)
                         ])
def test_09(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
