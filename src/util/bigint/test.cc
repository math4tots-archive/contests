// g++ -std=c++11 test.cc && ./a.out
#include "bigint.hh"
#include <assert.h>

int main() {
  assert(BigInt(100).str() == "100");
  assert(BigInt(1000000).str() == "1000000");
  assert(BigInt(1000000).pow(4).number_of_digits() == 3);
  assert(BigInt(1000000) + BigInt(1000008) == BigInt(1000000 + 1000008));
  assert(BigInt(17) * BigInt(724) == BigInt(17 * 724));
  std::cout << BigInt(1000000).pow(2).str() << std::endl;
  assert(BigInt(1000000).pow(2).str() == "1000000000000");
  assert(BigInt(71484) - BigInt(901400) == BigInt(71484 - 901400));
  assert((BigInt(0) - BigInt(4)).str() == "-4");
}
