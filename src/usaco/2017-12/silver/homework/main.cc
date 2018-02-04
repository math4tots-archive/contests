#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>

typedef long long LLong;

template <class T> T gcd(T a, T b) {
  while (b) {
    T t = a % b;
    a = b;
    b = t;
  }
  return a;
}

std::vector<LLong> solve(const std::vector<LLong> &scores) {
  LLong N = scores.size();
  std::vector<LLong> best_ks;

  LLong bestnum = 0;
  LLong bestden = 1;
  LLong total = scores.back();
  LLong min = scores.back();

  for (LLong K = N - 2; K >= 1; K--) {
    total += scores[K];
    min = std::min(min, scores[K]);
    LLong num = total - min;
    LLong den = N - K - 1;
    LLong g = gcd(num, den);
    num /= g;
    den /= g;

    if (num * bestden > den * bestnum) {
      best_ks.clear();
      bestnum = num;
      bestden = den;
    }

    if (num * bestden == den * bestnum) {
      best_ks.push_back(K);
    }
  }

  std::sort(best_ks.begin(), best_ks.end());
  return best_ks;
}

int main() {
  std::ifstream fin("homework.in");
  std::ofstream fout("homework.out");
  LLong N;
  fin >> N;
  std::vector<LLong> scores;
  for (LLong i = 0; i < N; i++) {
    LLong score;
    fin >> score;
    scores.push_back(score);
  }

  std::vector<LLong> ks = solve(scores);

  for (LLong i = 0; i < ks.size(); i++) {
    fout << ks[i] << std::endl;
  }
}