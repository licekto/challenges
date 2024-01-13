from .utility import *
from advent_of_code.day_24 import *


MIN = 200000000000000
MAX = 400000000000000


@pytest.mark.parametrize('file_path, lo, hi, exp1, exp2',
                         [(['24', 'ref.in'], 7, 27, 2, 47),
                          (['24', 'user.in'], MIN, MAX, 23760, 888708704663413)])
def test_24(get_data, file_path, lo, hi, exp1, exp2):
    assert (solve(parse(get_data(file_path)), lo, hi) == (exp1, exp2))
