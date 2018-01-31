def reqsteps(n):
    "Number of steps required to bring n to zero"
    # The strategy is to use as many 5s as possible,
    # then as many 2s as possible,
    # then use the remaining 1 as necessary
    return n // 5 + n % 5 // 2 + n % 5 % 2


def solve(cc):
    cc = tuple(cc)

    # 'normalize' first, so that lowest value is zero
    lower_bound = min(cc)
    cc = [c - lower_bound for c in cc]

    # The offset here determines what the final value that everyeone has
    # at the end should be.
    # Because using more than two 1s or using more than two 2s can be
    # replaced to use more 5s, we should never need to consider
    # offsets larger than 5.
    return min(sum(reqsteps(c + offset) for c in cc) for offset in range(6))


assert solve([2, 2, 3, 7]) == 2

t = int(input())
for _ in range(t):
    input()  # read n
    print(solve(map(int, input().split())))
