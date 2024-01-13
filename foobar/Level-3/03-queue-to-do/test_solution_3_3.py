from solution_3_3 import *
import pytest


@pytest.mark.parametrize('start, length, exp',
                         [(17, 4, 14),
                          (0, 3, 2),
                          (0, 1, 0),
                          (123, 1, 123),
                          (123, 2, 122),
                          (0, 10000, 82460672)])
def test(start, length, exp):
    assert(solution(start, length) == exp)

