import heapq
import itertools

_removed = object()


class Heap(object):
    def __init__(self):
        self.counter = itertools.count()
        self.array = []
        self.entry_finder = dict()

    def __setitem__(self, key, priority):
        self.discard(key)
        entry = [priority, next(self.counter), key]
        heapq.heappush(self.array, entry)
        self.entry_finder[key] = entry

    def __getitem__(self, key):
        return self.entry_finder[key][0]

    def __contains__(self, key):
        return key in self.entry_finder

    def __len__(self):
        return len(self.entry_finder)

    def discard(self, key):
        if key in self.entry_finder:
            self.entry_finder[key][-1] = _removed
            del self.entry_finder[key]

    def peek(self):
        while self.array[0][-1] is _removed:
            heapq.heappop(self.array)

        return self.array[0][-1]

    def pop(self):
        key = self.peek()
        self.discard(key)
        return key


class BitField(object):
    def __init__(self, value=0):
        if isinstance(value, BitField):
            value = value.value

        self.value = value

    def add(self, n):
        self.value |= 1 << n

    def discard(self, n):
        bn = 1 << n
        if self.value & bn:
            self.value -= bn

    def __hash__(self):
        return hash(self.value)

    def __eq__(self, other):
        return self.value == other.value

    def __or__(self, other):
        return BitField(self.value | other.value)

    def __repr__(self):
        return ''.join(map(str, ((self.value >> i) % 2 for i in range(K))))


def main():
    global K
    N, M, K = map(int, input().split())

    ftypes = []
    for _ in range(N):
        bf = BitField()
        for fish_type in list(map(int, input().split()))[1:]:
            bf.add(fish_type - 1)  # index from 0
        ftypes.append(bf.value)

    cost = [dict() for _ in range(N)]

    def connect(X, Y, Z):
        if Y in cost[X]:
            cost[X][Y] = min(Z, cost[X][Y])
        else:
            cost[X][Y] = Z

    for _ in range(M):
        X, Y, Z = map(int, input().split())
        X -= 1  # Index everyone from 0 instead of 1
        Y -= 1
        connect(X, Y, Z)
        connect(Y, X, Z)
    # Heap contains entries:
    #  (current-node, fish-types, cost-so-far)
    expanded = dict()
    heap = Heap()
    heap[0, ftypes[0]] = 0
    at_end = dict()  # maps fish-types -> cost

    while heap:
        x, f = heap.peek()
        c = heap[x, f]
        heap.pop()
        expanded[x, f] = c
        if x == N - 1:
            at_end[f] = c

        for y in cost[x]:
            key = (y, f | ftypes[y])
            if key not in expanded and (key not in heap or c < heap[key]):
                heap[key] = c + cost[x][y]

    targetbf = BitField()
    for i in range(K):
        targetbf.add(i)
    target = targetbf.value

    answer = float('infinity')
    for bf_a in at_end:
        cost_a = at_end[bf_a]
        for bf_b in at_end:
            cost_b = at_end[bf_b]
            # print('%s %s %s %s' % (bf_a, bf_b, bf_a | bf_b,
            #                        max(cost_a, cost_b)))
            if bf_a | bf_b == target:
                answer = min(answer, max(cost_a, cost_b))

    # print(at_end)
    print(answer)


main()
