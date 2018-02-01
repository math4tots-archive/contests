"""
ID: math4to3
LANG: PYTHON3
TASK: beads
"""
import itertools


def merge(color1, color2):
    if 'w' in (color1, color2):
        if any(c != 'w' for c in (color1, color2)):
            return color1 if color2 == 'w' else color2
        else:
            return 'w'

    if color1 == color2:
        return color1

    return None


def solve(beads):
    # If there's only one color, the choice doesn't matter
    if len(set(beads)) == 1:
        return len(beads)

    # Otherwise, first rearrange so that the left and right side
    # are guaranteed to have different colors.
    spliti = next(i for i in range(len(beads)) if beads[i] != beads[i + 1])
    beads = beads[spliti + 1:] + beads[:spliti + 1]

    # The two max consecutive set of groups is the answer.
    groups = [(key, len(list(elements)))
              for key, elements in itertools.groupby(beads)]

    best = 0

    def decr(i):
        if i == 0:
            return len(groups) - 1
        else:
            return i - 1

    def incr(i):
        if i == len(groups) - 1:
            return 0
        else:
            return i + 1

    for i in range(len(groups)):
        total = 0

        # Take going left, being careful not to double take
        # if we happen to wrap around.
        li = i
        lcolor = groups[li][0]
        total += groups[li][1]
        li = decr(li)

        while li != i and merge(lcolor, groups[li][0]):
            total += groups[li][1]
            lcolor = merge(lcolor, groups[li][0])
            li = decr(li)

        # Take from the right side, being carful not to
        # take anything that's already been taken on the way going left.
        ri = incr(i)
        rcolor = 'w'
        while ri != incr(li) and merge(rcolor, groups[ri][0]):
            total += groups[ri][1]
            rcolor = merge(rcolor, groups[li][0])
            ri = incr(ri)

        best = max(best, total)

    return best


assert solve('wwwbbrwrbrbrrbrbrwrwwrbwrwrrb') == 11

with open('beads.in') as f:
    f.readline()  # read N
    beads = f.readline().strip()

with open('beads.out', 'w') as f:
    f.write('{}\n'.format(solve(beads)))
