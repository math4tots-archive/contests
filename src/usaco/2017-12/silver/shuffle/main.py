def main():
    with open('shuffle.in', 'r') as f:
        N = int(f.readline())
        sh = [int(word) - 1 for word in f.readline().split()]

    mark = [None] * N
    stack = list(range(N))
    while stack:
        i = stack.pop()
        if mark[i] is not None:
            continue

        seen = {i}
        path = [i]
        i = sh[i]
        while i not in seen and mark[i] is None:
            seen.add(i)
            path.append(i)
            i = sh[i]

        if mark[i] is None:
            # we've hit a cycle -- now we can mark everyone we've
            # seen so far
            cycle_start = path.index(i)
            for j in path[:cycle_start]:
                mark[j] = 0
            for j in path[cycle_start:]:
                mark[j] = 1
        else:
            # If we encounter a value that has already been marked,
            # either 'tail' or 'cycle', it means that
            # the values we've been looking at so far cannot be
            # part of a cycle.
            for j in path:
                mark[j] = 0

    with open('shuffle.out', 'w') as f:
        f.write('{}\n'.format(sum(mark)))


main()
