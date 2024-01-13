from .utility import *
from advent_of_code.day_25 import *


@pytest.mark.parametrize('file_path, exp_p1, exp_p2',
                         [(['25', 'ref-adjusted.in'], 54, 0),
                          (['25', 'user-adjusted.in'], 543256, 0)
                          ])
def test_25(get_data, file_path, exp_p1, exp_p2):
    assert (solve(parse(get_data(file_path))) == exp_p1)

