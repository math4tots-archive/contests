// WIP

#ifndef avltree_hh
#define avltree_hh

#include <stddef.h>
#include <string>
#include <utility>

template <class Key> struct AvlTree {
  typedef bool Dir;
  // LEFT -> false
  // RIGHT -> true

  AvlTree(): rt(NULL) {}

 private:
  struct Error {
    std::string message;
    Error(const std::string &m): message(m) {}
  };

  struct Node {
    Node *children[2], *parent;
    Key key;
    size_t height;

    Node(Key k): key(k), height(1) {
      children[0] = children[1] = parent = nullptr;
    }
  };

  // Location is a (parent, direction) pair.
  typedef std::pair<Node*, Dir> Location;

  Node *rt;

  void rotate(Node *n, Dir dir) {
    //  Overview of the transformation (right rotate):
    //
    //        p
    //        n
    //    nn      c
    // a     b
    //
    //       |
    //       v
    //
    //        p
    //        nn
    //    a        n
    //           b    c
    //
    Dir right = dir;
    Dir left = 1 - dir;

    if (!n) {
      throw Error("Tried to rotate null node");
    }
    Node *p = n->parent;
    Dir pdir = (p && p->children[true] == n) ? true : false;

    Node *nn = n->children[left];
    if (!nn) {
      throw Error("Tried to rotate with null child");
    }
    Node *b = nn->children[right];

    connect(p, pdir, nn);
    connect(nn, right, n);
    connect(n, left, b);
  }

  // Find either
  //   - the location of node containing key if key already present, or
  //   - the location of where a new node should be created to insert
  //     this key.
  Location find_location(const Key &key) const {
    Node *parent = NULL;
    Dir pdir = false;
    Node *node = rt;

    while (node) {
      Dir nextdir;
      if (key < node->key) {
        nextdir = false;  // search more left
      } else if (node->key < key) {
        nextdir = true;  // search more right
      } else {
        return std::make_pair(parent, pdir);
      }
      parent = node;
      pdir = nextdir;
      node = parent->children[pdir];
    }

    return std::make_pair(parent, pdir);
  }

  // Connects a parent node with a child node in the given direction.
  // If parent is nullptr, we assume we're assigning root, and
  // ignore dir.
  void connect(Node *parent, Dir dir, Node *child) {
    if (child) {
      child->parent = parent;
    }
    if (parent) {
      parent->children[dir] = child;
    } else {
      rt = child;
    }
  }

  Node *get(Location loc) const {
    if (loc->first) {
      return loc->first->children[loc->second];
    }
    return rt;
  }

  size_t heightof(Node *node) {
    if (node) {
      return node->height;
    }
    return 0;
  }

  void rebalance(Node *node) {
    while (node) {
      size_t heights[2];
      heights[0] = heightof(node->children[0]);
      heights[1] = heightof(node->children[1]);
      Dir tcdir = heights[0] > heights[1] ? 0 : 1;  // bigger child dir;
      size_t computed_height = heights[tcdir] + 1;
      if (computed_height == )
      node = node->parent;
    }
  }

  // Should only be called when loc points to a null node,
  // otherwise, we'll leak resources.
  Node *create_node(Location loc, const Key &key) {
    Node *parent = loc->first;
    Dir pdir = loc->second;
    Node *node = new Node(key);
    node->parent = parent;
    return node;
  }

 public:
  void add(const Key &key) {
    if (!get(find_location(key))) {
      create_node(loc, key);
    }
  }
};

#endif//avltree_hh
