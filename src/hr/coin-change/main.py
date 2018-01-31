"""Classic DP problem -- number of ways to get target amount from
given denominations.
"""


def nways(n: int, coins: [int]):
    "Returns number of ways to make the value 'n' with the given coin types"

    coins = sorted(set(coins))

    if len(coins) == 0:
        return 1 if n == 0 else 0

    memo = [[0 for _ in range(n + 1)] for coin in coins]

    # special case 1: exactly one way to make zero, regardless of
    # number of coins.
    for arr in memo:
        arr[0] = 1

    # special case 2: using exactly 1 coin
    for total in range(0, n + 1, coins[0]):
        memo[0][total] = 1

    # inductive step
    for i, coin in list(enumerate(coins))[1:]:
        for total in range(min(coin, n + 1)):
            memo[i][total] = memo[i - 1][total]

        for total in range(coin, n + 1):
            memo[i][total] = memo[i - 1][total] + memo[i][total - coin]

    return memo[-1][-1]


assert nways(4, [1, 2, 3]) == 4
assert nways(10, [2, 5, 3, 6]) == 5

n, _ = map(int, input().split())
coins = map(int, input().split())
print(nways(n, coins))
