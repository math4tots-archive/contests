/*
ID: math4to3
TASK: milk2
LANG: C++14
*/
#include <set>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>
using I = long long;

struct Range final {
  I start, end;

  Range(I a, I b): start(a), end(b) {}

  bool operator==(const Range &other) const {
    return start == other.start && end == other.end;
  }

  bool operator<(const Range &other) const {
    return start < other.start || (start == other.start && end < other.end);
  }

  bool contains(I x) const {
    return start <= x && x <= end;
  }

  bool overlaps(const Range &other) const {
    return
      contains(other.start) ||
      contains(other.end) ||
      other.contains(start) ||
      other.contains(end);
  }

  void absorb(const Range &other) {
    start = std::min(start, other.start);
    end = std::max(end, other.end);
  }
};

template <class T> T incr(T iter) {
  ++iter;
  return iter;
}

template <class T> T decr(T iter) {
  --iter;
  return iter;
}

struct Ranges final {
  std::set<Range> rr;

  void add(Range r) {
    auto start_iter = rr.lower_bound(r);

    auto left_iter = start_iter;
    for (; left_iter != rr.begin() && r.overlaps(*decr(left_iter));
         --left_iter) {
      r.absorb(*decr(left_iter));
    }

    auto right_iter = start_iter;
    for (; right_iter != rr.end() && r.overlaps(*right_iter); ++right_iter) {
      r.absorb(*right_iter);
    }

    rr.erase(left_iter, right_iter);
    rr.insert(r);
  }

  void add(I a, I b) {
    add(Range(a, b));
  }

  I longest_milked() const {
    I longest = 0;
    for (auto range: rr) {
      longest = std::max(longest, range.end - range.start);
    }
    return longest;
  }

  I longest_idle() const {
    if (rr.size() < 2) {
      return 0;
    }
    I longest = 0;
    for (auto iter = rr.begin(); incr(iter) != rr.end(); ++iter) {
      longest = std::max(longest, incr(iter)->start - iter->end);
    }
    return longest;
  }
};


int main() {
  {
    Ranges ranges;
    ranges.add(300, 1000);
    ranges.add(700, 1200);
    ranges.add(1500, 2100);
    assert(ranges.longest_milked() == 900);
    assert(ranges.longest_idle() == 300);
  }

  std::ifstream fin("milk2.in");
  std::ofstream fout("milk2.out");
  I N;
  fin >> N;
  Ranges ranges;
  for (I i = 0; i < N; i++) {
    I start, end;
    fin >> start >> end;
    ranges.add(start, end);
  }

  fout << ranges.longest_milked() << " " << ranges.longest_idle() << std::endl;
}
