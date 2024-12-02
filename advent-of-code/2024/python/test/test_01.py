from .utility import *
from advent_of_code.day_01 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['01', 'ref.in'], 11, 31),
                          (['01', 'user.in'], 936063, 23150395)])
def test_01(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))

