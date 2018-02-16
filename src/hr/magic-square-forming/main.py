def _permutation_helper(xs, end):
    if end < 2:
        yield
    else:
        yield from _permutation_helper(xs, end - 1)
        for i in reversed(range(end - 1)):
            xs[i], xs[end - 1] = xs[end - 1], xs[i]
            yield from _permutation_helper(xs, end - 1)
            xs[i], xs[end - 1] = xs[end - 1], xs[i]


def permutations(xs):
    xs = list(xs)
    for _ in _permutation_helper(xs, len(xs)):
        yield tuple(xs)


magic_sum = sum(range(1, 10)) // 3


def is_magic(xs):
    for i in range(3):
        if xs[3 * i] + xs[3 * i + 1] + xs[3 * i + 2] != magic_sum:
            return False

        if xs[i] + xs[i + 3] + xs[i + 6] != magic_sum:
            return False

    if xs[0] + xs[4] + xs[8] != magic_sum:
        return False

    if xs[2] + xs[4] + xs[6] != magic_sum:
        return False

    return True


def diff(xs, ys):
    return sum(abs(x - y) for x, y in zip(xs, ys))


def costs(xs):
    for perm in permutations(range(1, 10)):
        if is_magic(perm):
            yield diff(xs, perm)


def solve(xs):
    return min(costs(xs))


print(
    solve([
        x for _ in range(3) for words in input().split()
        for x in map(int, words)
    ]))
