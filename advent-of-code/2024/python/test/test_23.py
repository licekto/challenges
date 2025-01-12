from .utility import *
from advent_of_code.day_23 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [(['23', 'ref.in'], 7, 'co,de,ka,ta'),
                      (['23', 'user.in'], 1156, 'bx,cx,dr,dx,is,jg,km,kt,li,lt,nh,uf,um')
                     ])
def test_23(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

