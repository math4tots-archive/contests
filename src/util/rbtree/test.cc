#include "rbtree.hh"
#include <assert.h>
#include <iostream>
#include <string>


// You could argue that all the
//   assert(tree.debugstr() == "...")
// below are bad because they are making assertions about
// implementation details of the tree --
// that is, correct implementations of an rbtree with minor differences
// would fail these tests.
// This is true, but at the same time, I find that these debugstr tests
// are really good documentation, and also provides a very simple
// and convenient way of showing that the tree seems to be
// still rebalancing itself. Other more accurate and general
// ways of showing that the tree is being balanced exist,
// (e.g. by testing a bound on the height of the tree, or
// doing a performance test on certain operations), but these are
// significantly more involved to get right, and don't seem like they'd
// be as informative to someone new reading the tests as
// something that will show you exactly what the tree looks like.

// NOTE: tested with clang++ (Apple LLVM version 9.0.0 (clang-900.0.39.2))
//       with options
//         -Weverything -Werror --std=c++11 -Wno-padded -Wno-c++98-compat

int main() {
  std::vector<std::string> vals({"a", "b", "c", "d", "e"});
  RbTree<std::string> tree(vals.begin(), vals.end());

  // Each node is represented by <key>:<subtree-size>.
  // The parentheses show the structure of which nodes are children
  // of which other nodes.
  assert(tree.debugstr() == "RbTree((a:1)b:5((c:1)d:3(e:1)))");

  // Random access
  assert(tree.size() == 5);
  assert(tree[0] == "a");
  assert(tree[1] == "b");
  assert(tree[2] == "c");
  assert(tree[3] == "d");
  assert(tree[4] == "e");

  // Add an entry "in the middle"
  tree.add("aa");
  assert(tree.size() == 6);
  assert(tree[0] == "a");
  assert(tree[1] == "aa");
  assert(tree[2] == "b");
  assert(tree[3] == "c");
  assert(tree[4] == "d");
  assert(tree[5] == "e");
  assert(tree.debugstr() == "RbTree((a:2(aa:1))b:6((c:1)d:3(e:1)))");

  // Remove an element
  tree.erase("b");
  assert(tree.size() == 5);
  assert(tree[0] == "a");
  assert(tree[1] == "aa");
  assert(tree[2] == "c");
  assert(tree[3] == "d");
  assert(tree[4] == "e");
  assert(tree.has("a"));
  assert(!tree.has("b"));
  assert(tree.debugstr() == "RbTree((a:2(aa:1))c:5(d:2(e:1)))");

  // Basic sanity test that the iterator conforms to STL rules
  assert(
    std::vector<std::string>(tree.begin(), tree.end()) ==
    std::vector<std::string>({"a", "aa", "c", "d", "e"}));

  // Test that tree.find() works
  assert(
    std::vector<std::string>(tree.find("c"), tree.end()) ==
    std::vector<std::string>({"c", "d", "e"}));
}

