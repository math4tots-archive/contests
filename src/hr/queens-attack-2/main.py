directions = (
    (1, 1),
    (-1, 1),
    (1, -1),
    (-1, -1),
    (0, 1),
    (0, -1),
    (1, 0),
    (-1, 0),
)


class Point(object):
    def __init__(self, row, col):
        self.row = row
        self.col = col

    def __iter__(self):
        yield self.row
        yield self.col

    def __sub__(self, other):
        return Path(self.row - other.row, self.col - other.col)

    def path_to_point(self, other):
        return other - self

    def path_to_edge(self, direction, board):
        nsteps = float('infinity')
        for x, dirx, upper_limit in zip(self, direction, board):
            if dirx == 1:
                nsteps = min(nsteps, upper_limit - x)
            elif dirx == -1:
                # NOTE: We index from 1, as per problem statement
                nsteps = min(nsteps, x - 1)
            elif dirx == 0:
                pass
            else:
                assert False, dirx

        dr, dc = [nsteps * x for x in direction]
        return Path(dr, dc)

    def __repr__(self):
        return 'Point({}, {})'.format(self.row, self.col)


def normalize(x):
    return 0 if x == 0 else x // int(abs(x))


class Path(object):
    def __init__(self, dr, dc):
        self.dr = dr
        self.dc = dc

    @property
    def has_valid_direction(self):
        return len(set(map(abs, (self.dr, self.dc))) - {0}) <= 1

    @property
    def direction(self):
        return (normalize(self.dr), normalize(self.dc))

    def __len__(self):
        assert self.has_valid_direction
        return max(map(int, map(abs, (self.dr, self.dc))))

    def __repr__(self):
        return 'Path({}, {})'.format(self.dr, self.dc)

    def __eq__(self, other):
        return (self.dr, self.dc) == (other.dr, other.dc)


assert Path(5, 0).has_valid_direction
assert Point(1, 1).path_to_edge((1, 1), (3, 10)) == Path(2, 2)
assert Point(1, 1).path_to_edge((1, -1), (8, 8)) == Path(0, 0)
assert len(Path(0, 0)) == 0
assert len(Path(10, 0)) == 10
assert len(Path(5, 5)) == 5


def solve(queen: Point, board: (int, int), obstacles: [Point], debug=False):
    spread = {d: len(queen.path_to_edge(d, board)) for d in directions}

    def display_status():
        for d, val in spread.items():
            print('{} -> {}'.format(d, val))

    if debug:
        print('Queen at {}'.format(queen))
        display_status()

    for obstacle in obstacles:
        path = queen.path_to_point(obstacle)

        if not path.has_valid_direction:
            if debug:
                print('ignoring out of the way obstacle {} ({})'.format(
                    obstacle, path))
            continue

        d = path.direction
        spread[d] = min(spread[d], len(path) - 1)

        if debug:
            print('processing {}'.format(obstacle))
            print('path = {}'.format(path))
            display_status()

    return sum(spread.values())


assert solve(Point(4, 4), (4, 4), []) == 9
assert solve(Point(4, 3), (5, 5),
             [Point(5, 5), Point(4, 2), Point(2, 3)]) == 10

n, k = map(int, input().split())
queen = Point(*map(int, input().split()))
obstacles = [Point(*map(int, input().split())) for _ in range(k)]

print(solve(queen, (n, n), obstacles))
