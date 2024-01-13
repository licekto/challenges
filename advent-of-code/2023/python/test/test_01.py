from .utility import *
from advent_of_code.day_01 import *


@pytest.mark.parametrize('file_path, expected',
                         [(['01', 'ref.in'], 281),
                          (['01', 'user.in'], 53866)])
def test_01(get_data, file_path, expected):
    assert(solve(get_data(file_path)) == expected)
