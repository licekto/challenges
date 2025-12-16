from .utility import *
from advent_of_code.day_05 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['05', 'ref.in'], 3, 14),
                          (['05', 'user.in'], 520, 347338785050515)
                         ])
def test_05(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))
