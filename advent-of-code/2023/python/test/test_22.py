from .utility import *
from advent_of_code.day_22 import *


@pytest.mark.parametrize('file_path, exp1, exp2',
                         [(['22', 'ref.in'], 5, 7),
                          (['22', 'user.in'], 459, 75784)
                          ])
def test_22(get_data, file_path, exp1, exp2):
    assert (solve(*parse(get_data(file_path))) == (exp1, exp2))
