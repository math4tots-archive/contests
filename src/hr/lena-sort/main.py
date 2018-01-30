"""
Solution idea:

The list to be sorted naturally forms a binary tree, where the roots
of each subtree is the pivot for that subtree.

For example, the list

5 1 2 8 7

becomes

      5
  1       8
   2     7

The total number of comparison is equal to the sum of the number of ancestors
each node has.

So in the example above:

5 -> 0
1 -> 1
2 -> 2
8 -> 1
7 -> 2
------
     6 comparisons


TODO: Finish this explanation...

"""
LIM = 10**5 + 10


def min_comparison_for_list_of_length(n):
    ncomps = 0
    remaining = n
    depth = 0
    width = 1
    while remaining:
        to_remove = min(remaining, width)
        ncomps += to_remove * depth
        remaining -= to_remove
        depth += 1
        width *= 2
    return ncomps


def max_comparison_for_list_of_length(n):
    return n * (n - 1) // 2


lower_bound_of = list(map(min_comparison_for_list_of_length, range(LIM)))
upper_bound_of = list(map(max_comparison_for_list_of_length, range(LIM)))


class Tree(object):
    def build_list(self):
        arr = []

        stack = [(1, self)]
        while stack:
            offset, node = stack.pop()
            if type(node) == Branch:
                arr.append(offset + node.left.size)
                stack.append((offset + node.left.size + 1, node.right))
                stack.append((offset, node.left))
            elif node == leaf:
                arr.append(offset)

        return arr


class Branch(Tree):
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.size = left.size + right.size + 1


class _Void(Tree):
    size = 0


void = _Void()


class _Leaf(Tree):
    size = 1


leaf = _Leaf()


class Impossible(Tree):
    def build_list(self):
        return [-1]


full_tree_cache = {
    0: void,
    1: leaf,
    2: Branch(leaf, void),
    3: Branch(leaf, leaf),
}

tree_cache = dict()

tree_cache[(0, 0)] = void
tree_cache[(1, 0)] = leaf
tree_cache[(2, 1)] = Branch(leaf, void)

# precompute upper bound trees
last_key = (2, 1)
for n in range(3, LIM):
    key = (n, upper_bound_of[n])
    tree_cache[key] = Branch(tree_cache[last_key], void)

    last_key = key


def get_tree(n, c):
    key = (n, c)
    if key not in tree_cache:
        tree = build_tree(n, c)
        tree_cache[key] = build_tree(n, c)
    return tree_cache[key]


def find_split(n, c):
    c -= n - 1
    for nleft in range((n - 1) // 2 + 1):
        nright = n - 1 - nleft
        lower = lower_bound_of[nleft] + upper_bound_of[nright]
        upper = upper_bound_of[nleft] + upper_bound_of[nright]
        if lower <= c <= upper:
            return ((nleft, c - upper_bound_of[nright]),
                    (nright, upper_bound_of[nright]))
    for nleft in reversed(range((n - 1) // 2 + 1)):
        nright = n - 1 - nleft
        lower = lower_bound_of[nleft] + lower_bound_of[nright]
        upper = upper_bound_of[nleft] + upper_bound_of[nright]
        if lower <= c <= upper:
            if upper_bound_of[nleft] + lower_bound_of[nright] <= c:
                return ((nleft, upper_bound_of[nleft]),
                        (nright, c - upper_bound_of[nleft]))
            else:
                return ((nleft, c - lower_bound_of[nright]),
                        (nright, lower_bound_of[nright]))


def build_tree(n, c):
    if c < lower_bound_of[n] or c > upper_bound_of[n]:
        return Impossible()

    if n in (0, 1):
        return leaf

    # 'depth' trick used here is an optimization.
    # Whenever we can stretch out the tree into just a line, we
    # choose to do so.
    original_n, original_c = n, c
    depth = 0
    while (n > 3
           and lower_bound_of[n - 1] <= c - (n - 1) <= upper_bound_of[n - 1]):
        depth += 1
        c -= n - 1
        n -= 1

    (nleft, cleft), (nright, cright) = find_split(n, c)
    node = Branch(get_tree(nleft, cleft), get_tree(nright, cright))

    for _ in range(depth):
        node = Branch(node, void)

    return node


assert Branch(Branch(leaf, leaf), leaf).build_list() == [4, 2, 1, 3, 5]

# print(Branch(Branch(leaf, void), leaf).build_list())


def solve(n, c):
    return ' '.join(map(str, build_tree(n, c).build_list()))


# print(solve(5, 6))
# print(solve(5, 100))

for _ in range(int(input())):
    n, c = map(int, input().split())
    print(solve(n, c))

# print(lower_bound_of[100000])
# print(upper_bound_of[100000])
