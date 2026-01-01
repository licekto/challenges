from .utility import *
from advent_of_code.day_11 import *


@pytest.mark.parametrize('file_path, expected',
                         [(['11', 'ref1.in'], 5),
                          (['11', 'user.in'], 511)
                         ])
def test_11_1(get_data, file_path, expected):
    assert(solve1(get_data(file_path)) == expected)


@pytest.mark.parametrize('file_path, expected',
                         [(['11', 'ref2.in'], 2),
                          (['11', 'user.in'], 458618114529380)
                         ])
def test_11_2(get_data, file_path, expected):
    assert(solve2(get_data(file_path)) == expected)
