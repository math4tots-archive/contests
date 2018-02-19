def solve(ladders):
    cost = [float('infinity')] * 100
    cost[0] = 0
    i = 0
    while i < len(cost):
        next_i = i + 1
        for roll in range(1, 7):
            dest = i + roll

            if dest >= 100:
                dest = i
            elif dest in ladders:
                dest = ladders[dest]

            if cost[i] + 1 < cost[dest]:
                cost[dest] = cost[i] + 1
                next_i = min(next_i, dest)
        i = next_i

    return -1 if cost[-1] == float('infinity') else cost[-1]


for _ in range(int(input())):
    ladders = dict()
    for _ in range(int(input())):
        start, end = map(int, input().split())
        start -= 1
        end -= 1
        ladders[start] = end

    for _ in range(int(input())):
        start, end = map(int, input().split())
        start -= 1
        end -= 1
        ladders[start] = end

    print(solve(ladders))
