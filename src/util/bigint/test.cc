// g++ -std=c++11 test.cc && ./a.out
#include "bigint.hh"
#include <assert.h>

int main() {
  assert(BigInt(100).str() == "100");
  assert(BigInt(1000000).str() == "1000000");
  assert(BigInt(1000000).pow(4).number_of_digits() == 3);
  assert(BigInt(1000000) + BigInt(1000008) == BigInt(1000000 + 1000008));
  assert(BigInt(17) * BigInt(724) == BigInt(17 * 724));
  assert(BigInt(1000000).pow(2).str() == "1000000000000");
  assert(BigInt(71484) - BigInt(901400) == BigInt(71484 - 901400));
  assert((BigInt(0) - BigInt(4)).str() == "-4");
  assert((BigInt(1000000).pow(2) / 10).str() == "100000000000");
  assert((BigInt(1000000).pow(2) % 10).str() == "0");
  assert((BigInt(1000000).pow(2) / 17).str() == "58823529411");
  assert((BigInt(1000000).pow(2) % 17).str() == "13");

  {
    auto a = BigInt(177777);
    auto b = a.pow(7);
    assert(a == a);
    assert(b == b);
    assert(a < b);
    assert(b / a == a.pow(6));
    assert(b % a == 0);

    assert(a.pow(0).str() == "1");
    assert(a.pow(1).str() == "177777");
    assert(a.pow(2).str() == "31604661729");
    assert(a.pow(3).str() == "5618581948196433");
    assert(a.pow(4).str() == "998854643004517269441");
    assert(a.pow(5).str() == "177573381869414066609412657");
    assert(a.pow(6).str() == "31568463108598824519621553923489");
    assert(a.pow(7).str() == "5612146666057373226624760991856103953");

    assert(a % 17 == 8);
    assert(a / 17 == 10457);

    assert((a.pow(7) / (a + 1).pow(3)).str() == "998837787448298872760");
    assert((a.pow(7) % (a + 1).pow(3)).str() == "5618013058596433");
  }
}
