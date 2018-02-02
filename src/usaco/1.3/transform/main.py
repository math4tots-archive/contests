"""
ID: math4to3
LANG: PYTHON3
TASK: transform
"""


class Pattern(object):
    def __init__(self, grid):
        if isinstance(grid, str):
            grid = grid.strip().splitlines()

        self._grid = list(map(list, grid))

    def __eq__(self, other):
        return self._grid == other._grid

    @property
    def ncol(self):
        return len(self._grid[0])

    @property
    def nrow(self):
        return len(self._grid)

    def rotate90(self):
        return Pattern([[
            self._grid[self.nrow - 1 - cno][rno] for cno in range(self.nrow)
        ] for rno in range(self.ncol)])

    def rotate(self, n):
        for _ in range(n):
            self = self.rotate90()
        return self

    def reflect(self):
        return Pattern([[
            self._grid[rno][self.ncol - 1 - cno] for cno in range(self.ncol)
        ] for rno in range(self.nrow)])

    def __str__(self):
        return '\n'.join(''.join(row) for row in self._grid) + '\n'

    def compare(self, other):
        for i in (1, 2, 3):
            if self.rotate(i) == other:
                return i

        if self.reflect() == other:
            return 4

        if any(self.reflect().rotate(i) == other for i in (1, 2, 3)):
            return 5

        if self == other:
            return 6

        return 7


assert Pattern("""
@-@
---
@@-
""").rotate90() == Pattern("""
@-@
@--
--@
""")

assert Pattern("""
@-@
---
@@-
""").compare(Pattern("""
@-@
@--
--@
""")) == 1

with open('transform.in') as f:
    n = int(f.readline())
    p1 = Pattern([f.readline().strip() for _ in range(n)])
    p2 = Pattern([f.readline().strip() for _ in range(n)])

with open('transform.out', 'w') as f:
    f.write('{}\n'.format(p1.compare(p2)))
