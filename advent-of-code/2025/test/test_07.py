from .utility import *
from advent_of_code.day_07 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['07', 'ref1.in'], 21, 40),
                          (['07', 'user.in'], 1585, 16716444407407)
                         ])
def test_07(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
