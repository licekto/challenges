from .utility import *
from advent_of_code.day_19 import *


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['19', 'ref.in'], 19114, 167409079868000),
                          (['19', 'user.in'], 476889, 132380153677887)
                          ])
def test_19(get_data, file_path, expected_part1, expected_part2):
    workflow, ratings = parse(get_data(file_path))
    assert (solve_p1(workflow, ratings) == expected_part1)
    assert (solve_p2(workflow) == expected_part2)
