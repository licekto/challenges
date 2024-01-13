from solution_1_1 import *
import pytest


@pytest.mark.parametrize('s, n',
                         [('', 0),
                          ('a', 1),
                          ('aa', 2),
                          ('aaa', 3),
                          ('abab', 2),
                          ('abccbaabccba', 2),
                          ('abcabcabcabc', 4),
                          ('aba', 1),
                          ('ab', 1),
                          ('aabaaba', 1),
                          ('abaababaab', 2),
                          ('abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz', 4),
                          ('abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy', 1),
                          ('aaabbcaaabbccaaabbcaaabbcc', 2),
                          ('aaabbcaaabbccaaabbcaaabbc', 1)])
def test(s, n):
    assert(solution(s) == n)

