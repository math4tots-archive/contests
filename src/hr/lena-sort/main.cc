/*
plan

1. precompute min/max possible ncomps for each length,
2. Given length and ncomps, build tree
3. From tree, build a sample list with desired length and ncomps.

*/
#define MAX_N 100000
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
// #include <memory>

struct Node;
typedef long long I;

// Because the structures we build are so long,
// using shared_ptr sometimes causes a stackoverflow when we exit.
// Since this is just a contest problem, I'm just gonna let
// resources leak here.
// typedef std::shared_ptr<Node> N;

typedef Node *N;

I lower_bound[MAX_N + 1];
I upper_bound[MAX_N + 1];


struct Node /* : public std::enable_shared_from_this<Node> */ {
  virtual ~Node() {}

  virtual std::vector<I> build_list() {
    std::vector<I> arr;

    std::vector<std::pair<I, N> > stack;  // (offset, Node) pairs

    // The list that we generate will have smallest element as 1 (not 0).
    // stack.push_back(std::make_pair(1, shared_from_this()));
    stack.push_back(std::make_pair(1, this));

    while (!stack.empty()) {
      std::pair<I, N> pair = stack.back();
      stack.pop_back();

      I offset = pair.first;
      N node = pair.second;

      node->process_for_build_list(offset, arr, stack);
    }

    return arr;
  }

  virtual I size() const=0;

 protected:
  virtual void process_for_build_list(
      I, std::vector<I>&, std::vector<std::pair<I, N> >&) {}
};

struct Branch: public Node {
  const N left, right;
  const I siz;
  Branch(N a, N b): left(a), right(b), siz(1 + left->size() + right->size()) {}

  void process_for_build_list(I offset, std::vector<I> &arr,
                              std::vector<std::pair<I, N> > &stack) {
    arr.push_back(offset + left->size());
    stack.push_back(std::make_pair(offset + left->size() + 1, right));
    stack.push_back(std::make_pair(offset, left));
  }

  I size() const { return siz; }
};

struct Leaf: public Node {
  void process_for_build_list(I offset, std::vector<I> &arr,
                              std::vector<std::pair<I, N> > &stack) {
    arr.push_back(offset);
  }

  I size() const { return 1; }
};

struct Empty: public Node {
  I size() const { return 0; }
};

struct Impossible: public Node {
  std::vector<I> build_list() {
    std::vector<I> ret;
    ret.push_back(-1);
    return ret;
  }

  I size() const {
    throw "fubar";
  }
};

N leaf(new Leaf());
N empty(new Empty());
N impossible(new Impossible());

N br(N a, N b) {
  if (a == impossible || b == impossible) {
    throw "fubar";
  }
  // return std::make_shared<Branch>(a, b);
  return new Branch(a, b);
}

std::map<std::pair<I, I>, N> tree_cache;

N build_tree(I len, I ncomps);

N build_compound_tree_without_opts(I len, I ncomps) {
  if (len == 1) {
    return leaf;
  }

  if (len == 0) {
    return empty;
  }

  // account for the comparisons that must happen at the root
  // before looking at the left or right subtrees.
  ncomps -= len - 1;

  for (I llen = (len - 1) / 2; llen >= 0; llen--) {
    I rlen = len - 1 - llen;

    I lmin = lower_bound[llen];
    I lmax = upper_bound[llen];
    I rmin = lower_bound[rlen];
    I rmax = upper_bound[rlen];

    if (ncomps < lmin + rmin || ncomps > rmax + lmax) {
      continue;
    }

    if (ncomps >= lmin + rmax) {
      return br(build_tree(llen, ncomps - rmax), build_tree(rlen, rmax));
    } else if (ncomps >= lmax + rmin) {
      return br(build_tree(llen, lmax), build_tree(rlen, ncomps - lmax));
    } else {
      return br(build_tree(llen, lmin), build_tree(rlen, ncomps - lmin));
    }

    throw "fubar";
  }

  throw "fubar";
}

N build_tree(I len, I ncomps) {
  if (ncomps < lower_bound[len] || ncomps > upper_bound[len]) {
    return impossible;
  }

  if (tree_cache.find(std::make_pair(len, ncomps)) != tree_cache.end()) {
    return tree_cache.find(std::make_pair(len, ncomps))->second;
  }

  // Set up depth optimization
  I depth = 0;
  const I original_len = len;
  const I original_ncomps = ncomps;

  while (len > 3 &&
         ncomps - (len - 1) >= lower_bound[len - 1] &&
         ncomps - (len - 1) <= upper_bound[len - 1]) {
    ncomps -= (len - 1);
    len--;
    depth++;
  }

  N node = empty;

  if (tree_cache.find(std::make_pair(len, ncomps)) != tree_cache.end()) {
    node = tree_cache.find(std::make_pair(len, ncomps))->second;
  } else {
    node = build_compound_tree_without_opts(len, ncomps);
    tree_cache.insert(std::make_pair(std::make_pair(len, ncomps), node));
  }

  // Finish up depth optimization
  for (I i = 0; i < depth; i++) {
    node = br(node, empty);
  }

  tree_cache.insert(std::make_pair(
    std::make_pair(original_len, original_ncomps), node));

  return node;
}

std::string str(std::vector<I> v) {
  std::stringstream ss;
  ss << "[";
  for (size_t i = 0; i < v.size(); i++) {
    ss << v[i] << ", ";
  }
  ss << "]";
  return ss.str();
}

void precompute() {
  // precompute bounds
  I next_power_of_2 = 2;
  I min_comps_per_new_node = 0;

  for (I i = 2; i <= MAX_N; i++) {

    if (i == next_power_of_2) {
      next_power_of_2 *= 2;
      min_comps_per_new_node++;
    }

    lower_bound[i] = lower_bound[i-1] + min_comps_per_new_node;
    upper_bound[i] = upper_bound[i-1] + i - 1;
  }

  // precompute tree_cache for the cases when ncomp == upper_bound[len]
  tree_cache.insert(std::make_pair(
      std::make_pair(1, upper_bound[1]), leaf));
  for (I len = 2; len <= MAX_N; len++) {
    tree_cache.insert(std::make_pair(
      std::make_pair(len, upper_bound[len]),
      br(
        tree_cache.find(std::make_pair(len - 1, upper_bound[len - 1]))->second,
        empty)));
  }
}

int main() {
  precompute();
  assert(empty == empty);
  assert(
    str(br(empty, br(br(leaf, leaf), br(leaf, empty)))->build_list()) ==
    "[1, 5, 3, 2, 4, 7, 6, ]");
  assert(
    str(build_tree(5, 6)->build_list()) ==
    "[3, 2, 1, 5, 4, ]");

  using namespace std;

  I q;
  cin >> q;
  for (I i = 0; i < q; i++) {
    I len, c;
    cin >> len >> c;
    vector<I> result = build_tree(len, c)->build_list();
    for (I j = 0; j + 1 < result.size(); j++) {
      cout << result[j] << " ";
    }
    if (!result.empty()) {
      cout << result[result.size() - 1] << std::endl;
    }
  }
}
