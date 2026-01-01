from .utility import *
from advent_of_code.day_06 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['06', 'ref1.in'], 4277556, 3263823),
                          (['06', 'user.in'], 3261038365331, 8342588849093)
                         ])
def test_06(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
