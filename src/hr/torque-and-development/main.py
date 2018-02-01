"""A little funny that the title says "Roads and Libraries"
even though the url seems to say 'torque-and-development'.

The problem screams minimum spanning tree.

---

It looks like some optimizations have to do be done here as
just using a dict for the Graph and UnionFind causes the program
to timeout on test cases.

---

All tests pass with the dict -> list optimization.
"""


class UnionFind(object):
    def __init__(self, n):
        self._map = [i for i in range(n)]
        self._size_map = [1 for _ in range(n)]

    def get(self, node):
        if self._map[self._map[node]] != self._map[node]:
            path = [node]
            while self._map[path[-1]] != path[-1]:
                path.append(self._map[path[-1]])
            for n in path:
                self._map[n] = path[-1]
            return path[-1]
        return self._map[node]

    def size_of(self, node):
        return self._size_map[self.get(node)]

    def join(self, a, b):
        a = self.get(a)
        b = self.get(b)
        if a == b:
            return

        smaller = min(a, b, key=self.size_of)
        larger = max(a, b, key=self.size_of)

        if smaller == larger:
            smaller, larger = a, b

        self._map[smaller] = larger
        self._size_map[larger] += self._size_map[smaller]


uf = UnionFind(3)
assert uf.size_of(1 - 1) == 1
assert uf.size_of(2 - 1) == 1
assert uf.size_of(3 - 1) == 1
uf.join(1 - 1, 2 - 1)
assert uf.size_of(1 - 1) == 2
assert uf.size_of(2 - 1) == 2
assert uf.size_of(3 - 1) == 1
assert uf.get(1 - 1) == uf.get(2 - 1)
assert uf.get(1 - 1) != uf.get(3 - 1)
uf.join(1 - 1, 3 - 1)
assert uf.get(2 - 1) == uf.get(3 - 1)
assert uf.size_of(3 - 1) == 3


class Graph(object):
    def __init__(self, n):
        self.n = n
        self.edges = []

    def join(self, a, b, weight):
        self.edges.append((a, b, weight))

    def mst(self):
        "computes mst by returning an iterable of edges"

        mst = []

        uf = UnionFind(self.n)
        self.edges.sort(key=lambda edge: edge[2])

        for a, b, weight in self.edges:
            if uf.get(a) != uf.get(b):
                uf.join(a, b)
                mst.append((a, b, weight))

        return mst


def solve(n, clib, croad, edges):
    graph = Graph(n + 1)

    for i in range(1, n + 1):
        graph.join(i, 0, clib)  # connect to fake 'build library' node

    for a, b in edges:
        graph.join(a, b, croad)

    return sum(edge[2] for edge in graph.mst())


assert solve(3, 2, 1, [(1, 2), (3, 1), (2, 3)]) == 4
assert solve(6, 2, 5, [(1, 3), (3, 4), (2, 4), (1, 2), (2, 3), (5, 6)]) == 12

for _ in range(int(input())):
    n, m, clib, croad = map(int, input().split())
    edges = [tuple(map(int, input().split())) for _ in range(m)]
    print(solve(n, clib, croad, edges))
