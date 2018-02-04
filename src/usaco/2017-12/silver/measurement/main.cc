#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <vector>
using I = long long;

struct Measurement final {
  I day, cow, diff;
  Measurement() {}
  Measurement(I d, I c, I f): day(d), cow(c), diff(f) {}
  bool operator<(const Measurement &m) const {
    return day < m.day;
  }
};

I solve(I G, std::vector<Measurement> &ms) {
  std::sort(ms.begin(), ms.end());

  std::map<I, I> out;  // maps cows to their current output.
  std::map<I, std::set<I>> gout;  // maps outputs to set of cows.

  auto highest_output = [&]() {
    return gout.rbegin()->first;
  };

  auto number_of_highest_output_cows = [&]() {
    return gout.rbegin()->second.size();
  };

  auto update = [&](I cow, I new_output) {

    if (out.find(cow) == out.end()) {
      out[cow] = G;
      gout[G].insert(cow);
    }

    // Update out
    I old_output = out[cow];
    if (old_output == new_output) {
      return;
    }
    out[cow] = new_output;

    // Update gout
    auto iter = gout.find(old_output);
    iter->second.erase(cow);
    if (iter->second.empty()) {
      gout.erase(iter);
    }
    gout[new_output].insert(cow);
  };

  // auto debug = [&]() {
  //   for (auto pair: out) {
  //     std::cout << pair.first << " -> " << pair.second << "  ";
  //   }
  //   std::cout << std::endl;
  // };

  update(-1, G);  // the other cow whose milk output level is always G.
  for (auto m: ms) {
    update(m.cow, G);  // set everyone's starting output level to G.
  }

  I nadjustments = 0;
  // debug();
  for (auto m: ms) {
    if (m.diff == 0) {
      continue;  // this should never happen however, as per problem statement
    }

    I old_highest = highest_output();
    I old_output = out[m.cow];
    I new_output = old_output + m.diff;
    I old_number_of_highest_output_cows = number_of_highest_output_cows();
    update(m.cow, new_output);
    I new_highest = highest_output();
    I new_number_of_highest_output_cows = number_of_highest_output_cows();

    // The highest output value changed, but who is on top has not.
    if (old_highest == old_output && new_highest == new_output &&
        old_number_of_highest_output_cows == 1 &&
        new_number_of_highest_output_cows == 1) {
      continue;
    }

    // The current cow whose value is updated was neither the highest
    // before the measurement or after. This means that the change cannot
    // affect who is on display.
    if (old_highest != old_output && new_highest != new_output) {
      continue;
    }

    // If we make it this far, it means that this cow has held the highest
    // output, or hold it now or both.
    // If it's not both, this cows picture must either be put up or taken
    // down, so there must be some adjustment.
    // If both, either the highest went up or down.
    // If it went up, there must be at least one cow besides this one
    // whose picture must come down (because of the check we did above).
    // If down, there is at least one cow besides this one whose picture
    // must go up.
    nadjustments++;

    // debug();
  }

  return nadjustments;
}


int main() {
  std::ifstream fin("measurement.in");
  std::ofstream fout("measurement.out");
  I N, G;
  std::vector<Measurement> ms;

  fin >> N >> G;
  for (I i = 0; i < N; i++) {
    Measurement m;
    fin >> m.day >> m.cow >> m.diff;
    ms.push_back(m);
  }

  fout << solve(G, ms) << std::endl;
}
