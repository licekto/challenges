from .utility import *
from advent_of_code.day_12 import *


@pytest.mark.parametrize('row, nums',
                         [('#.#.###', [1, 1, 3]),
                          ('.#...#....###', [1, 1, 3]),
                          ('.#.###.#.######', [1, 3, 1, 6]),
                          ('####.#...#...', [4, 1, 1]),
                          ('#....######..#####.', [1, 6, 5]),
                          ('.###.##....#', [3, 2, 1])
                          ])
def test_combinations_no_unknown(row, nums):
    assert (solve_combinations(row, nums) == 1)


@pytest.mark.parametrize('row, nums, expected',
                         [('??.#', [1, 1], 2),
                          ('???.###', [1, 1, 3], 1),
                          ('??.??.#', [1, 1, 1], 4),
                          ('??.?#', [1, 1], 2),
                          ('.??..??...###.', [1, 1, 3], 4),
                          ('.??..??...?##.', [1, 1, 3], 4),
                          ('?#?#?#?#?#?#?#?', [1, 3, 1, 6], 1),
                          ('????.#...#...', [4, 1, 1], 1),
                          ('????.######..#####.', [1, 6, 5], 4),
                          ('????.##', [1, 2], 4),
                          ('#.??.##', [1, 2], 1),
                          ('??.##', [2], 1),
                          ('#.??.##.#', [1, 2, 1], 1),
                          ('?###????????', [3, 2, 1], 10),
                          ('??????', [2, 1], 6),
                          ('????', [1, 1], 3)
                          ])
def test_combinations(row, nums, expected):
    assert (solve_combinations(row, nums) == expected)


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['12', 'ref.in'], 21, 525152),
                          (['12', 'user.in'], 7047, 17391848518844)
                          ])
def test_12(get_data, file_path, expected_part1, expected_part2):
    assert (solve(parse(get_data(file_path))) == (expected_part1, expected_part2))
