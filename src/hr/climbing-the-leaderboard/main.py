import itertools

input()  # n
scores = [float('infinity')
          ] + [k for k, _ in itertools.groupby(map(int,
                                                   input().split()))] + [-1]
input()  # m
alice = tuple(map(int, input().split()))


def find_index(score):
    """If i = find_index(score), then
       scores[i - 1] > score >= scores[i]

       This returned value should coincide with the desired
       ranking.
    """
    lower = 0
    upper = len(scores) - 1

    while lower + 1 < upper:
        mid = (lower + upper) // 2

        if score >= scores[mid]:
            upper = mid
        else:
            lower = mid

    return upper


for score in alice:
    print(find_index(score))
