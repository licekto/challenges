from .utility import *
from advent_of_code.day_08 import *


@pytest.mark.parametrize('file_path, expected_part2',
                         [(['08', 'ref_part2.in'], 6),
                          (['08', 'user.in'], 21165830176709)])
def test_08(get_data, file_path, expected_part2):
    assert (search_n(*parse(get_data(file_path))) == expected_part2)

