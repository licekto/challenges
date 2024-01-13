from .utility import *
from advent_of_code.day_23 import *


@pytest.mark.parametrize('file_path, exp1, exp2',
                         [(['23', 'ref.in'], 94, 154),
                          (['23', 'user.in'], 2190, 6258)
                          ])
def test_23(get_data, file_path, exp1, exp2):
    assert(solve(get_data(file_path)) == (exp1, exp2))
