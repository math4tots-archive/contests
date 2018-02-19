def solve(B):
    if len(B) < 2:
        return 0

    lower = upper = 0
    last_b = B[0]

    for b in B[1:]:
        next_lower = max(upper + abs(last_b - 1), lower)
        next_upper = max(lower + abs(b - 1), upper + abs(b - last_b))
        lower, upper, last_b = next_lower, next_upper, b

    return max(lower, upper)


def main():
    for _ in range(int(input())):
        input()  # N
        print(solve(list(map(int, input().split()))))


main()
