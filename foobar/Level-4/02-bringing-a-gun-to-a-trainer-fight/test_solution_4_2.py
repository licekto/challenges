from solution_4_2 import *
import pytest


@pytest.mark.parametrize('dim, self, trainer, dst, expected',
                         [([3, 2], [1, 1], [2, 1], 4, 7),
                          ([300, 275], [150, 150], [185, 100], 500, 9)
                         ])
def test(dim, self, trainer, dst, expected):
    assert(solution(dim, self, trainer, dst) == expected)
