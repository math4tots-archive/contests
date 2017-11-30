
class Piece(object):
    def solve(self):
        if self.initials_count > 1:
            a, b = self._split()
            a.solve()
            b.solve()
        elif self.initials_count == 1:
            self._fill()
        else:
            raise Exception("FUBAR initials_count = 0")

    def slice_by_row(self, r):
        a = self.slice(0, 0, r, self.width)
        b = self.slice(r, 0, self.height, self.width)
        return a, b

    def slice_by_col(self, c):
        a = self.slice(0, 0, self.height, c)
        b = self.slice(0, c, self.height, self.width)
        return a, b

    def _split(self):
        for row in range(1, self.height):
            a, b = self.slice_by_row(row)
            if a.initials_count and b.initials_count:
                return a, b

        for col in range(1, self.width):
            a, b = self.slice_by_col(col)
            if a.initials_count and b.initials_count:
                return a, b

        raise Exception("FUBAR")

    def _split_by(self, orientation):
        pass

    def _fill(self):
        initial = ''
        for r in range(self.height):
            for c in range(self.width):
                if self[r, c] != '?':
                    initial = self[r, c]
                    break

            if initial:
                break

        for r in range(self.height):
            for c in range(self.width):
                self[r, c] = initial


class Cake(Piece):
    def __init__(self, grid):
        self.grid = [[initial for initial in row] for row in grid]
        self.cum = [[0 for initial in row] for row in grid]

        for r, row in enumerate(self.grid):
            for c, entry in enumerate(row):
                value = 0 if entry == '?' else 1
                if r > 0:
                    value += self.cum[r-1][c]
                if c > 0:
                    value += self.cum[r][c-1]
                if r > 0 and c > 0:
                    value -= self.cum[r-1][c-1]
                self.cum[r][c] = value

    def slice(self, row0, col0, row1, col1):
        return Slice(self, row0, col0, row1, col1)

    def __getitem__(self, rc):
        r, c = rc
        return self.grid[r][c]

    def __setitem__(self, rc, value):
        r, c = rc
        self.grid[r][c] = value

    @property
    def initials_count(self):
        return self.cum[-1][-1]

    @property
    def height(self):
        return len(self.grid)

    @property
    def width(self):
        return len(self.grid[0])


class Slice(Piece):
    def __init__(self, cake, row0, col0, row1, col1):
        self.cake = cake
        self.row0 = row0
        self.col0 = col0
        self.row1 = row1
        self.col1 = col1

    @property
    def initials_count(self):
        total = self.cake.cum[self.row1 - 1][self.col1 - 1]
        if self.row0 > 0:
            total -= self.cake.cum[self.row0 - 1][self.col1 - 1]
        if self.col0 > 0:
            total -= self.cake.cum[self.row1 - 1][self.col0 - 1]
        if self.row0 > 0 and self.col0 > 0:
            total += self.cake.cum[self.row0 - 1][self.col0 - 1]
        return total

    def slice(self, row0, col0, row1, col1):
        return Slice(
            self.cake,
            self.row0 + row0, self.col0 + col0,
            self.row0 + row1, self.col0 + col1,
        )

    def __getitem__(self, rc):
        r, c = rc
        return self.cake[r + self.row0, c + self.col0]

    def __setitem__(self, rc, value):
        r, c = rc
        self.cake[r + self.row0, c + self.col0] = value

    @property
    def height(self):
        return self.row1 - self.row0

    @property
    def width(self):
        return self.col1 - self.col0

def solve(grid):
    cake = Cake(grid)
    cake.solve()
    return ''.join(''.join(row) + '\n' for row in cake.grid).strip()

def testcase(input_, output):
    assert(solve(input_.strip().splitlines()) == output.strip())


testcase(
"""
A?
??
""",
"""
AA
AA
"""
)

testcase("""
??A
???
""",
"""
AAA
AAA
"""
)

testcase("""
??A
?B?
""",
"""
AAA
BBB
"""
)

testcase("""
??A
?B?
???
???
C??
""",
"""
AAA
BBB
CCC
CCC
CCC
"""
)

T = int(input())
for t in range(1, T + 1):
    R, C = map(int, input().split())
    grid = []
    for _ in range(R):
        grid.append(input().strip())

    print('Case #%s:\n%s' % (t, solve(grid)))
