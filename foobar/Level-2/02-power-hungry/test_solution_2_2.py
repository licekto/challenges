from solution_2_2 import *
import pytest


@pytest.mark.parametrize('arr, exp',
                         [([2, 0, 2, 2, 0], '8'),
                          ([-2, -3, 4, -5], '60'),
                          ([2, -3, 1, 0, -5], '30'),
                          ([0], '0'),
                          ([1], '1'),
                          ([-1], '-1'),
                          ([-1, 1], '1'),
                          ([-2, -1, 1], '2'),
                          ([-2, -1], '2'),
                          ([-2, -5, 4], '40'),
                          ([-2, -5, 4, -1], '40'),
                          ([1000] * 50, f'{1000 ** 50}'),
                          ([0, -1, 1], '1'),
                          ([0, 0, 0, 1, 1], '1'),
                          ([0, 0, -10, 3, 0, 0, 5], '15'),
                          ([0, 0, -10, -3], '30'),
                          ([0, 0, -8, -3, -4], '32'),
                          ([-1000] * 50, f'{1000 ** 50}'),
                          ([-1000] * 3, f'{1000 ** 2}'),
                          ([-1000] * 9, f'{1000 ** 8}'),
                          ([-1000] * 49, f'{1000 ** 48}')])
def test(arr, exp):
    assert(solution(arr) == exp)

