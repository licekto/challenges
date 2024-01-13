from solution_3_1 import *
import pytest


@pytest.mark.parametrize('m, f, exp',
                         [('4', '7', '4'),
                          ('2', '1', '1'),
                          ('2', '4', 'impossible'),
                          ('500001', '1', '500000'),
                          ('51', '100', '27'),
                          (str(10**50), str(10**20 + 1), '1000000000000000000009999999999'),
                          (str(10**50), str(10**20), 'impossible')])
def test(m, f, exp):
    assert(solution(m, f) == exp)

