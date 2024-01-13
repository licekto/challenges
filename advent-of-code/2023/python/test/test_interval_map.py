import pytest

from advent_of_code.interval_map import *


def test_interval_map():
    interval_map = IntervalMap()
    assert(str(interval_map) == 'IntervalMap: {}')
    assert(10 not in interval_map)

    it = iter(interval_map)
    with pytest.raises(StopIteration):
        next(it)

    for i in range(4):
        interval_map[10] = 'a'
        assert (10 in interval_map)
        assert (str(interval_map) == "IntervalMap: {10: 'a'}")
        it = iter(interval_map)
        assert (it.get() == (10, 'a'))
        next(it)
        with pytest.raises(StopIteration):
            next(it)

    interval_map[5] = 'b'
    assert (10 in interval_map)
    assert (5 in interval_map)
    assert (str(interval_map) == "IntervalMap: {5: 'b', 10: 'a'}")
    it = iter(interval_map)
    assert(it.get() == (5, 'b'))

    interval_map = IntervalMap()
    interval_map[10] = 'a'
    interval_map[15] = 'b'
    assert (str(interval_map) == "IntervalMap: {10: 'a', 15: 'b'}")
    it = iter(interval_map)
    assert (it.get() == (10, 'a'))
    next(it)
    assert (it.get() == (15, 'b'))

    interval_map[5] = 'c'
    it = iter(interval_map)
    assert (it.get() == (5, 'c'))
    next(it)
    assert (it.get() == (10, 'a'))
    next(it)
    assert (it.get() == (15, 'b'))

    interval_map[7] = 'd'
    it = iter(interval_map)
    assert (it.get() == (5, 'c'))
    next(it)
    assert (it.get() == (7, 'd'))
    next(it)
    assert (it.get() == (10, 'a'))
    next(it)
    assert (it.get() == (15, 'b'))

    assert (interval_map.get_interval(4) is None)
    assert (interval_map.get_interval(5) == (5, 'c'))
    assert (interval_map.get_interval(6) == (5, 'c'))
    assert (interval_map.get_interval(7) == (7, 'd'))
    assert (interval_map.get_interval(8) == (7, 'd'))
    assert (interval_map.get_interval(9) == (7, 'd'))
    assert (interval_map.get_interval(10) == (10, 'a'))
    assert (interval_map.get_interval(15) == (15, 'b'))
    assert (interval_map.get_interval(16) == (15, 'b'))

    interval_map[12] = 'e'
    it = iter(interval_map)
    assert (it.get() == (5, 'c'))
    next(it)
    assert (it.get() == (7, 'd'))
    next(it)
    assert (it.get() == (10, 'a'))
    next(it)
    assert (it.get() == (12, 'e'))
    next(it)
    assert (it.get() == (15, 'b'))

    interval_map[18] = 'f'
    it = iter(interval_map)
    assert (it.get() == (5, 'c'))
    next(it)
    assert (it.get() == (7, 'd'))
    next(it)
    assert (it.get() == (10, 'a'))
    next(it)
    assert (it.get() == (12, 'e'))
    next(it)
    assert (it.get() == (15, 'b'))
    next(it)
    assert (it.get() == (18, 'f'))

    for i in range(-3, 23):
        interval_map[i] = chr(ord('a') + i + 3)

    for i, val in enumerate(interval_map):
        assert(val == chr(ord('a') + i))

    interval_map_str = "IntervalMap: {-3: 'a', -2: 'b', -1: 'c', 0: 'd', 1: 'e', 2: 'f', 3: 'g', 4: 'h', 5: 'i',"\
                       " 6: 'j', 7: 'k', 8: 'l', 9: 'm', 10: 'n', 11: 'o', 12: 'p', 13: 'q', 14: 'r', 15: 's', "\
                       "16: 't', 17: 'u', 18: 'v', 19: 'w', 20: 'x', 21: 'y', 22: 'z'}"
    assert(str(interval_map) == interval_map_str)

    lst = interval_map.to_list()
    assert(lst[0] == (-3, 'a'))
    assert(lst[25] == (22, 'z'))
