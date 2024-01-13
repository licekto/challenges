import copy, operator, re


class Rule:
    def __init__(self, variable, op, value, next_rule):
        self.variable = variable
        self.op = op
        self.__operator = operator.lt if op == '<' else operator.gt
        self.value = int(value)
        self.next_rule = next_rule

    def exec(self, other_value):
        return self.__operator(int(other_value), self.value)

    __call__ = exec


def parse(lines):
    workflow = {}

    i = 0
    while len(lines[i]) != 0:
        m = re.match(r'([a-z]+)\{(.*)\}', lines[i])
        name = m.group(1)
        rules = []
        for rule in m.group(2).split(','):
            m = re.match(r'([xmas])([<>])(\d+):([A-Za-z]+)', rule)
            if m is not None:
                rules.append(Rule(m.group(1), m.group(2), m.group(3), m.group(4)))
            else:
                rules.append(rule)
        workflow[name] = rules
        i += 1

    i += 1
    ratings = []
    while i < len(lines):
        ratings.append(dict(map(lambda r: (r[0], int(r[1])), re.findall(r'([xmas])=(\d+)', lines[i]))))
        i += 1

    return workflow, ratings


def solve_p1(workflow, ratings):
    res = 0
    for rating in ratings:
        rule_name = 'in'
        while rule_name != 'A' and rule_name != 'R':
            for rule in workflow[rule_name]:
                if type(rule) is not Rule:
                    rule_name = rule
                    break
                if rule(rating[rule.variable]):
                    rule_name = rule.next_rule
                    break
        if rule_name == 'A':
            res += sum(rating.values())

    return res


def solve_p2(workflow):
    mapping = {'x': 0, 'm': 1, 'a': 2, 's': 3}
    init_intervals = [[1, 4000], [1, 4000], [1, 4000], [1, 4000]]
    stack = [('in', copy.deepcopy(init_intervals))]

    res = 0
    while len(stack) != 0:
        rule_name, intervals = stack.pop()

        if rule_name == 'R':
            continue
        if rule_name == 'A':
            current = 1
            for lo, hi in intervals:
                current *= hi - lo + 1
            res += current
            continue

        for rule in workflow[rule_name]:
            if type(rule) is not Rule:
                stack.append((rule, copy.deepcopy(intervals)))
            elif rule.op == '<':
                prev_upper = intervals[mapping[rule.variable]][1]
                intervals[mapping[rule.variable]][1] = rule.value - 1

                stack.append((rule.next_rule, copy.deepcopy(intervals)))

                intervals[mapping[rule.variable]][0] = rule.value
                intervals[mapping[rule.variable]][1] = prev_upper
            else:
                prev_lower = intervals[mapping[rule.variable]][0]
                intervals[mapping[rule.variable]][0] = rule.value + 1

                stack.append((rule.next_rule, copy.deepcopy(intervals)))

                intervals[mapping[rule.variable]][0] = prev_lower
                intervals[mapping[rule.variable]][1] = rule.value

    return res
