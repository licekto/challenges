from .utility import *
from advent_of_code.day_07 import *


def test_wildcard():
    e = Evaluator()
    assert (e.get_type('QJJQ2', wildcard=True) == 6)
    assert (e.get_type('JKKK2', wildcard=True) == 6)
    assert(compare_hands('JKKK2', 'QQQQ2', wildcard=True) == -1)


def test_rank():
    e = Evaluator()
    assert(e.get_type('AAAAA') == 7)
    assert (e.get_type('AA8AA') == 6)
    assert (e.get_type('23332') == 5)
    assert (e.get_type('TTT98') == 4)
    assert (e.get_type('23432') == 3)
    assert (e.get_type('A23A4') == 2)
    assert (e.get_type('23456') == 1)


@pytest.mark.parametrize('h1, h2, expected',
                        [('23456', 'AAAAA', -1),
                         ('T55J5', 'KTJJT', 1),
                         ('23457', '34567', -1),
                         ('23457', '23457', 0),
                         ('45678', '34567', 1),
                         ('T3457', 'A3457', -1),
                         ('T3457', 'T3457', 0),
                         ('K3457', 'Q3457', 1),
                         ('AA8AA', 'AAAAA', -1),
                         ('AA8AA', 'AAA22', 1),
                         ('AAA22', 'AAA23', 1),
                         ('23432', 'AAA23', -1)])
def test_compare(h1, h2, expected):
    assert(compare_hands(h1, h2) == expected)


def test_sort():
    hands = ['32T3K', 'T55J5', 'KK677', 'KTJJT', 'QQQJA']
    hands.sort(key=functools.cmp_to_key(compare_hands))
    assert(hands == ['32T3K', 'KTJJT', 'KK677', 'T55J5', 'QQQJA'])


@pytest.mark.parametrize('file_path, expected_part1, expected_part2',
                         [(['07', 'ref.in'], 6440, 5905),
                          (['07', 'user.in'], 250602641, 251037509)
                         ])
def test_07(get_data, file_path, expected_part1, expected_part2):
    assert(solve(get_data(file_path)) == (expected_part1, expected_part2))
