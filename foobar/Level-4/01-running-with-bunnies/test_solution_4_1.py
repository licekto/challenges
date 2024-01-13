from solution_4_1 import *
import pytest


@pytest.mark.parametrize('times, times_limit, exp',
                         [([[0, 2, 2, 2, -1],
                            [9, 0, 2, 2, -1],
                            [9, 3, 0, 2, -1],
                            [9, 3, 2, 0, -1],
                            [9, 3, 2, 2,  0]], 1, [1, 2]),
                          ([[0, 1, 1, 1, 1],
                            [1, 0, 1, 1, 1],
                            [1, 1, 0, 1, 1],
                            [1, 1, 1, 0, 1],
                            [1, 1, 1, 1, 0]], 3, [0, 1])])
def test(times, times_limit, exp):
    assert(solution(times, times_limit) == exp)

