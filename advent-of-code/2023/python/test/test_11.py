from .utility import *
from advent_of_code.day_11 import *


@pytest.mark.parametrize('file_path, expand, expected',
                         [(['11', 'ref.in'], 2, 374),
                          (['11', 'ref.in'], 10, 1030),
                          (['11', 'ref.in'], 100, 8410),
                          (['11', 'user.in'], 2, 10228230),
                          (['11', 'user.in'], 1000000, 447073334102)
                          ])
def test_11(get_data, file_path, expand, expected):
    print()
    assert (solve(get_data(file_path), expand) == expected)
