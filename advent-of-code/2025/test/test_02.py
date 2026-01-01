from .utility import *
from advent_of_code.day_02 import *


@pytest.mark.parametrize('file_path, expected1, expected2',
                         [(['02', 'ref1.in'], 1227775554, 4174379265),
                          (['02', 'user.in'], 23560874270, 44143124633)
                         ])
def test_02(get_data, file_path, expected1, expected2):
    assert(solve(get_data(file_path)) == (expected1, expected2))

