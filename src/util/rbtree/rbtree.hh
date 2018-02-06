#ifndef rbtree_hh
#define rbtree_hh

// Red Black Tree
// Nodes also contain subtree sizes, for more algo fun.
// Basically verbatim from CLRS.
// Left/Right have been parameterized to reduce the amount of code
// to write though.
//
// Some modification from CLRS:
//   nodes keep track of subtree sizes.

// Note that duplicate keys are allowed.

// As a reminder, these are the properties of red-black trees (from CLRS)
//
//   1. Every node is either red or black.
//   2. The root is black.
//   3. Every leaf (NIL) is black.
//   4. If a node is red, then both its children are black.
//   5. For each node, all paths from the node to descendant leaves contain
//      the same number of black nodes.
//

// NOTE: Requires C++11

#include <vector>
#include <sstream>
#include <functional>
#include <initializer_list>

template <class Key, class Compare = std::less<Key> >
class RbTree {

 public:
  using size_type = long;
  using key_compare = Compare;

  static constexpr bool LEFT = false;
  static constexpr bool RIGHT = true;
  static constexpr bool RED = true;
  static constexpr bool BLACK = false;

 private:

  struct Node {
    Key key;
    Node *parent, *child[2];
    size_type subtree_size;
    bool color;

    Node():
        parent(this),
        child{this, this},
        subtree_size(0),
        color(BLACK) {}

    Node(const Key &k, Node *nilval):
        key(k),
        parent(nilval), child{nilval, nilval},
        subtree_size(1),
        color(RED) {}

    void debugstr(std::stringstream &ss) const {
      if (subtree_size) {
        ss << "(";
        child[LEFT]->debugstr(ss);
        ss << key << ":" << subtree_size;
        child[RIGHT]->debugstr(ss);
        ss << ")";
      }
    }
  };

  bool less(const Key &a, const Key &b, bool dir) {
    return dir == LEFT ? lt(a, b) : lt(b, a);
  }

  Compare lt;
  Node *const nil, *root;

  Node *treeSearch(Node *x, const Key &k) const {
    while (x != nil && k != x->key) {
      if (lt(k, x->key)) {
        x = x->child[LEFT];
      } else {
        x = x->child[RIGHT];
      }
    }
    return x;
  }

  // TODO: Implement this iteratively instead of recursively
  // Returns the Node with key whose value is closest to k but is not k
  // in the direction dir.
  Node *findLimitNode(Node *x, const Key &k, bool left) const {
    const bool right = !left;
    if (x == nil) {
      return nil;
    } else if (less(x->key, k, left)) {
      Node *r = findLimitNode(x->child[right], k, left);
      return r == nil ? x : r;
    } else {
      return findLimitNode(x->child[left], k, left);
    }
  }

  template <class N> N *ttreeExtrema(N *x, bool dir) const {
    while (x->child[dir] != nil) {
      x = x->child[dir];
    }
    return x;
  }

  Node *treeExtrema(Node *x, bool dir) const {
    return ttreeExtrema(x, dir);
  }

  const Node *treeExtrema(const Node *x, bool dir) const {
    return treeExtrema(x, dir);
  }

  template <class N> N *ttreeSuccessor(N *x, const bool right) const {
    const bool left = !right;

    if (x->child[right] != nil) {
      return treeExtrema(x->child[right], left);
    }
    N *y = x->parent;
    while (y != nil && x == y->child[right]) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  Node *treeSuccessor(Node *x, const bool right) const {
    return ttreeSuccessor(x, right);
  }

  const Node *treeSuccessor(const Node *x, const bool right) const {
    return ttreeSuccessor(x, right);
  }

  // NOTE: Assumes x and x->child[!left] are not nil.
  // when left=LEFT, the diagram is as follows:
  //
  //   x          rotate(x, LEFT)       y
  // a   y        ------------->     x    c
  //   b   c                       a   b
  //              <-------------
  //               rotate(y, RIGHT)
  //
  void rotate(Node *x, const bool left) {
    const bool right = !left;

    Node *y = x->child[right];

    // Update subtree sizes
    size_type a_subtree_size = x->child[left]->subtree_size;
    size_type c_subtree_size = y->child[right]->subtree_size;
    x->subtree_size -= 1 + c_subtree_size;
    y->subtree_size += 1 + a_subtree_size;

    x->child[right] = y->child[left];
    if (y->child[left] != nil) {
      y->child[left]->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil) {
      root = y;
    } else {
      if (x == x->parent->child[left]) {
        x->parent->child[left] = y;
      } else {
        x->parent->child[right] = y;
      }
    }
    y->child[left] = x;
    x->parent = y;
  }

  void rbInsert(Node *z) {
    Node *y = nil;
    Node *x = root;
    while (x != nil) {
      y = x;
      x = x->child[lt(z->key, x->key) ? LEFT : RIGHT];
    }
    z->parent = y;
    if (y == nil) {
      root = z;
    } else {
      if (lt(z->key, y->key)) {
        y->child[LEFT] = z;
      } else {
        y->child[RIGHT] = z;
      }
    }
    z->child[LEFT] = z->child[RIGHT] = nil;
    z->color = RED;
    subtreeSizeInsertFixup(z);
    rbInsertFixup(z);
  }

  void subtreeSizeInsertFixup(Node *z) {
    for (Node *i = z->parent; i != nil; i = i->parent) {
      i->subtree_size++;
    }
  }

  void rbInsertFixup(Node *z) {
    while (z->parent->color == RED) {
      const bool left =
          z->parent == z->parent->parent->child[LEFT] ? LEFT : RIGHT;
      const bool right = !left;

      Node *y = z->parent->parent->child[right];
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->child[right]) {
          z = z->parent;
          rotate(z, left);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rotate(z->parent->parent, right);
      }
    }
    root->color = BLACK;
  }

  Node *rbDelete(Node *z) {
    Node *y =
        (z->child[LEFT] == nil || z->child[RIGHT] == nil) ?
        z : treeSuccessor(z, RIGHT);
    subtreeSizeDeleteFixup(y);
    Node *x = y->child[LEFT] != nil ? y->child[LEFT] : y->child[RIGHT];
    x->parent = y->parent;
    if (y->parent == nil) {
      root = x;
    } else {
      if (y == y->parent->child[LEFT]) {
        y->parent->child[LEFT] = x;
      } else {
        y->parent->child[RIGHT] = x;
      }
    }
    if (y != z) {
      z->key = y->key;
    }
    if (y->color == BLACK) {
      rbDeleteFixup(x);
    }
    return y;
  }

  void subtreeSizeDeleteFixup(Node *y) {
    for (Node *i = y->parent; i != nil; i = i->parent) {
      i->subtree_size--;
    }
  }

  void rbDeleteFixup(Node *x) {
    while (x != root && x->color == BLACK) {
      const bool left = x == x->parent->child[LEFT] ? LEFT : RIGHT;
      const bool right = !left;

      Node *w = x->parent->child[right];
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotate(x->parent, left);
        w = x->parent->child[right];
      }
      if (w->child[left]->color == BLACK && w->child[right]->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->child[right]->color == BLACK) {
          w->child[left]->color = BLACK;
          w->color = RED;
          rotate(w, right);
          w = x->parent->child[right];
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->child[right]->color = BLACK;
        rotate(x->parent, left);
        x = root;
      }
    }
    x->color = BLACK;
  }

  template <class T, class N, class K>
  class ibase final {
    T *tree;
    N *node;

   public:

    //// types to make this a true 'InputIterator' under the eyes of
    //// the all powerful standard library.
    using difference_type = void;
    using value_type = Key;  // value_type never has const
    using pointer = K*;
    using reference = K&;
    using iterator_category = std::input_iterator_tag;

    ibase()=default;
    ibase(T *t, N *n): tree(t), node(n) {}
    ibase(const ibase&)=default;
    ~ibase()=default;
    ibase &operator=(const ibase&)=default;

    reference operator*() const {
      return node->key;
    }

    pointer *operator->() const {
      return &(node->key);
    }

    ibase &operator++() {
      node = tree->treeSuccessor(node, RIGHT);
      return *this;
    }

    ibase &operator++(int) {
      ibase ret = *this;
      ++*this;
      return ret;
    }

    ibase &operator--() {
      node = tree->treeSuccessor(node, LEFT);
      return *this;
    }

    ibase &operator--(int) {
      ibase ret = *this;
      --*this;
      return ret;
    }

    bool operator==(const ibase &other) const {
      return node == other.node;
    }

    bool operator!=(const ibase &other) const {
      return node != other.node;
    }

    N *getNode() const {
      return node;
    }
  };

 public:

  using iterator = ibase<RbTree, Node, Key>;
  using const_iterator = ibase<const RbTree, const Node, const Key>;

  RbTree(const Compare &comp = Compare()):
      lt(comp), nil(new Node()), root(nil) {}
  template <class InputIterator>
  RbTree(InputIterator first, InputIterator last,
         const Compare &comp = Compare()): RbTree(comp) {
    for (; first != last; ++first) {
      add(*first);
    }
  }
  RbTree(std::initializer_list<Key> keys, const Compare &comp = Compare()):
      RbTree(keys.begin(), keys.end(), comp) {}

  ~RbTree() {
    std::vector<Node*> stack(1, root);
    while (!stack.empty()) {
      Node *x = stack.back();
      stack.pop_back();
      if (x != nil) {
        stack.push_back(x->child[LEFT]);
        stack.push_back(x->child[RIGHT]);
        delete x;
      }
    }
    delete nil;
  }

  iterator getRoot() {
    return iterator(this, root);
  }

  iterator begin() {
    return iterator(this, treeExtrema(root, LEFT));
  }

  const_iterator begin() const {
    return const_iterator(this, treeExtrema(root, LEFT));
  }

  const_iterator cbegin() const {
    return begin();
  }

  iterator end() {
    return iterator(this, nil);
  }

  const_iterator end() const {
    return const_iterator(this, nil);
  }

  const_iterator cend() const {
    return end();
  }

  iterator find(const Key &key) {
    return iterator(this, treeSearch(root, key));
  }

  // findLimit(key, LEFT) returns an iterator to the node
  // with key with greatest value in tree that is strictly less
  // than given 'key'.
  iterator findLimit(const Key &key, bool dir) {
    return iterator(this, findLimitNode(root, key, dir));
  }

  const Key &front() /*const*/ {
    return *begin();
  }

  const Key &back() /*const*/ {
    return *end();
  }

  bool empty() const {
    return size() == 0;
  }

  size_type size() const {
    return root->subtree_size;
  }

  void add(const Key &key) {
    rbInsert(new Node(key, nil));
  }

  bool erase(const Key &key) {
    return erase(find(key));
  }

  bool erase(iterator x) {
    if (x == end()) {
      return false;
    }
    delete rbDelete(x.getNode());
    return true;
  }

  bool has(const Key &key) {
    return find(key) != end();
  }

  // Order statistic method -- get the i-th smallest element,
  // indexed from zero.
  const Key &operator[](size_type i) {
    return getiter(i).getNode()->key;
  }

  // Order statistic method -- get the i-th smallest element,
  // indexed from zero.
  iterator getiter(size_type i) {
    if (i < 0 || i >= size()) {
      return end();
    }
    Node *n = root;
    while (i != n->child[LEFT]->subtree_size) {
      if (i > n->child[LEFT]->subtree_size) {
        i -= 1 + n->child[LEFT]->subtree_size;
        n = n->child[RIGHT];
      } else {
        n = n->child[LEFT];
      }
    }
    return iterator(this, n);
  }

  std::string debugstr() const {
    std::stringstream ss;
    ss << "RbTree";
    root->debugstr(ss);
    return ss.str();
  }
};

#endif//rbtree_hh
