import re


def parse(lines):
    data = []
    for line in lines:
        row = re.match(r'([.#?]+)', line)
        nums = list(map(int, re.findall(r'(\d+)', line)))
        data.append((row.group(1), nums))
    return data


def add_if_not(dp, key, value):
    if key not in dp:
        dp[key] = value
    else:
        dp[key] = max(value, dp[key])


def get_combinations(row, ri, nums, ni, dp):
    if (ri, ni) in dp:
        return dp[(ri, ni)]

    while ri < len(row) and row[ri] == '.':
        ri += 1

    if ri == len(row):
        return 1 if ni == len(nums) else 0

    if ni == len(nums):
        while ri < len(row) and row[ri] != '#':
            ri += 1
        return 1 if ri == len(row) else 0

    combinations_count = 0
    current = nums[ni]
    orig_ri = ri

    while ri < len(row) and current != 0:
        if row[ri] == '?' and current == nums[ni]:
            combinations_count += get_combinations(row, ri + 1, nums, ni, dp)
            add_if_not(dp, (ri, ni), combinations_count)
        if row[ri] == '#' or row[ri] == '?':
            current -= 1
        else:
            break
        ri += 1

    if ri == len(row):
        if current == 0 and ni == len(nums) - 1:
            add_if_not(dp, (orig_ri, ni), combinations_count + 1)
            return combinations_count + 1
        else:
            add_if_not(dp, (orig_ri, ni), combinations_count)
            return combinations_count

    if current != 0 or row[ri] == '#':
        add_if_not(dp, (orig_ri, ni), combinations_count)
        return combinations_count

    if row[ri] == '?':
        ri += 1

    combinations_count += get_combinations(row, ri, nums, ni + 1, dp)
    add_if_not(dp, (orig_ri, ni), combinations_count)

    return combinations_count


def solve_combinations(row, nums):
    return get_combinations(row, 0, nums, 0, {})


def solve(data):

    res1, res2 = 0, 0
    for row, nums in data:
        res1 += solve_combinations(row, nums)
        res2 += solve_combinations((f'{row}?' * 4) + row, nums * 5)

    return res1, res2
