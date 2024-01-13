from .utility import *
from advent_of_code.day_17 import *


@pytest.mark.parametrize('file_path, expected',
                         [(['17', 'ref.in'], 102),
                          (['17', 'user.in'], 1256)
                          ])
def test_17_p1(get_data, file_path, expected):
    assert (solve_p1(get_data(file_path)) == expected)


@pytest.mark.parametrize('file_path, expected',
                         [(['17', 'ref.in'], 94),
                          (['17', 'user.in'], 1382)
                          ])
def test_17_p2(get_data, file_path, expected):
    assert (solve_p2(get_data(file_path)) == expected)
