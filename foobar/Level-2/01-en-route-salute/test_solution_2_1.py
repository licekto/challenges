from solution_2_1 import *
import pytest


@pytest.mark.parametrize('input, expected',
                         [('>----<', 2),
                          ('<<>><', 4),
                          ('--->-><-><-->-', 10),
                          ('>><<', 8)])
def test(input, expected):
    assert(solution(input) == expected)

