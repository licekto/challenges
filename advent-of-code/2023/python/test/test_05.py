from .utility import *
from advent_of_code.day_05 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['05', 'ref.in'], 35, 46),
                          (['05', 'user.in'], 510109797, 9622622)
                            ])
def test_05(get_data, file_path, expected_part1, expected_part2):
    assert(solve(*parse(get_data(file_path))) == (expected_part1, expected_part2))

