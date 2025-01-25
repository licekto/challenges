from .utility import *
from advent_of_code.day_24 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                     [
                      #(['24', 'ref.in'], 2024, None),
                      (['24', 'user.in'], 53190357879014, 'bks,hnd,nrn,tdv,tjp,z09,z16,z23')
                     ])
def test_24(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))

