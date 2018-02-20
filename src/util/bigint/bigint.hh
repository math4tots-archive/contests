#ifndef bigint_hh
#define bigint_hh

// Requires C++11

#include <assert.h>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct BigInt final {
  using Digit = long;
  using Digit2 = long long;
  using Sign = bool;
  using Digits = std::deque<Digit>;

  static constexpr const Digit BASE = 1000000000;
  static constexpr const Digit DIGIT_WIDTH = 9;  // should be log10(BASE)
  static constexpr const Sign PLUS = true;
  static constexpr const Sign MINUS = false;

  Sign sign;
  Digits digits;

  BigInt(): sign(PLUS) {}
  BigInt(Digit2 i) {
    sign = i >= 0 ? PLUS : MINUS;
    if (i < 0) {
      i = -i;
    }
    while (i) {
      digits.push_back(i % BASE);
      i /= BASE;
    }
    remove_extraneous_zeros(digits);
  }
  explicit BigInt(Sign s, Digits dd): sign(s), digits(std::move(dd)) {
    remove_extraneous_zeros(digits);
    if (digits.empty()) {
      sign = PLUS;
    }
  }

  bool operator==(const BigInt &other) const {
    return sign == other.sign && digits == other.digits;
  }

  bool operator<(const BigInt &other) const {
    if (sign != other.sign) {
      return sign < other.sign;
    }
    if (sign == MINUS) {
      return lt(other.digits, digits);
    }
    return lt(digits, other.digits);
  }

  explicit operator bool() const { return !digits.empty(); }

  BigInt pow(size_t n) const {
    if (n == 0) {
      return 1;
    } else if (n == 1) {
      return *this;
    } else if (n % 2 == 0) {
      auto i = pow(n / 2);
      return i * i;
    }
    auto i = pow(n / 2);
    return i * i * *this;
  }

  BigInt operator+(const BigInt &b) const {
    // A + B or -A + -B
    if (sign == b.sign) {
      return BigInt(sign, add(digits, b.digits));
    }
    // A + (-B)
    if (sign == PLUS) {
      if (compare(digits, b.digits) >= 0) {
        return BigInt(PLUS, subtract(digits, b.digits));
      }
      return BigInt(MINUS, subtract(b.digits, digits));
    }
    // -A + B
    if (compare(digits, b.digits) >= 0) {
      return BigInt(MINUS, subtract(digits, b.digits));
    }
    return BigInt(PLUS, subtract(b.digits, digits));
  }

  BigInt operator*(const BigInt &b) const {
    // TODO: Use better algorithm than long multiplication
    Digits dd(digits.size() + b.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < b.digits.size(); j++) {
        auto carry = static_cast<Digit2>(digits[i]) *
                     static_cast<Digit2>(b.digits[j]);
        auto pos = i + j;
        while (carry) {
          Digit2 digit = dd[pos];
          digit += carry;

          Digit2 next_carry = 0;
          if (digit >= BASE) {
            next_carry = digit / BASE;
            digit %= BASE;
          }
          dd[pos] = digit;

          carry = next_carry;
          pos++;
        }
      }
    }
    return BigInt(sign == b.sign ? PLUS : MINUS, std::move(dd));
  }

  BigInt operator-(const BigInt &b) const {
    return *this + -b;
  }

  BigInt operator-() const {
    return BigInt(!sign, digits);
  }

  std::pair<BigInt, BigInt> divmod(const BigInt &b) const {
    auto pair = divide(digits, b.digits);
    Sign outsign = sign == b.sign ? PLUS : MINUS;
    return std::make_pair(
      BigInt(outsign, std::move(pair.first)),
      BigInt(outsign, std::move(pair.second)));
  }

  BigInt operator/(const BigInt &b) const {
    return divmod(b).first;
  }

  BigInt operator%(const BigInt &b) const {
    return divmod(b).second;
  }

  std::string str() const {
    std::stringstream ss;
    write(ss);
    return ss.str();
  }

  void write(std::ostream &out) const {
    if (digits.empty()) {
      out << "0";
      return;
    }
    if (sign == MINUS) {
      out << "-";
    }
    for (auto iter = digits.rbegin(); iter != digits.rend(); ++iter) {
      std::stringstream ss;
      ss << *iter;
      auto digitstr = ss.str();

      if (iter != digits.rbegin()) {
        assert(digitstr.size() <= DIGIT_WIDTH);
        for (size_t j = 0; j < DIGIT_WIDTH - digitstr.size(); j++) {
          out << "0";
        }
      }

      out << digitstr;
    }
  }

  size_t number_of_digits() const {
    return digits.size();
  }

 private:

  void shift(size_t n) {
    for (size_t i = 0; i < n; i++) {
      digits.push_front(0);
    }
  }

  static void remove_extraneous_zeros(Digits &digits) {
    while (!digits.empty() && digits.back() == 0) {
      digits.pop_back();
    }
  }

  void flip_sign() {
    sign = !sign;
  }

  static int compare(const Digits &a, const Digits &b) {
    if (a.size() != b.size()) {
      return a.size() < b.size() ? -1 : 1;
    }
    auto aiter = a.rbegin();
    auto biter = b.rbegin();
    while (aiter != a.rend()) {
      auto da = *aiter;
      auto db = *biter;
      if (da != db) {
        return da < db ? -1 : 1;
      }
      ++aiter;
      ++biter;
    }
    return 0;
  }

  static bool lt(const Digits &a, const Digits &b) {
    return compare(a, b) < 0;
  }

  static bool eq(const Digits &a, const Digits &b) {
    return compare(a, b) == 0;
  }

  static void iadd(Digits &a, const Digits &b) {
    Digit carry = 0;
    size_t i = 0;

    while (i < a.size() || i < b.size() || carry) {
      if (i >= a.size()) {
        a.push_back(0);
      }
      Digit next = a[i] + (i < b.size() ? b[i] : 0) + carry;
      carry = next / BASE;
      a[i] = next % BASE;
      i++;
    }

    remove_extraneous_zeros(a);
  }

  // NOTE: You must know that a >= b.
  // If a < b, behavior is undefined.
  static void isubtract(Digits &a, const Digits &b) {
    Digit borrow = 0;
    Digits dd;
    size_t i = 0;

    while (i < b.size() || borrow) {
      assert(i < a.size());  // if false, b > a
      Digit2 digit = a[i];
      Digit bval = i < b.size() ? b[i] : 0;
      digit -= bval;
      digit -= borrow;
      borrow = 0;
      while (digit < 0) {
        borrow++;
        digit += BASE;
      }
      a[i] = static_cast<Digit>(digit);
      i++;
    }

    remove_extraneous_zeros(a);
  }

  static Digits add(const Digits &a, const Digits &b) {
    Digits ret = a;
    iadd(ret, b);
    return ret;
  }

  static Digits subtract(const Digits &a, const Digits &b) {
    Digits ret = a;
    isubtract(ret, b);
    return ret;
  }

  // If b == 0, undefined behavior
  // Returns (Quotient, Remainder) pair
  static std::pair<Digits, Digits> divide(const Digits &a, const Digits &b) {
    // TODO: Use a faster division algorithm.

    // Division by zero
    assert(!b.empty());

    // 0 / D
    if (a.empty()) {
      return std::make_pair(Digits(), Digits());
    }

    Digits quo, rem;

    if (a.size() == 1 && b.size() == 1) {
      quo.push_back(a[0] / b[0]);
      rem.push_back(a[0] % b[0]);
      remove_extraneous_zeros(quo);
      remove_extraneous_zeros(rem);
      return std::make_pair(quo, rem);
    }

    // dpow2 -> D, 2D, 2^2*D, 2^3*D, ...
    // pow2 -> 1, 2, 2^2, 2^3, ...
    std::vector<Digits> dpow2(1, b);
    std::vector<Digits> pow2(1, Digits({1}));
    while (lt(dpow2.back(), a)) {
      dpow2.push_back(add(dpow2.back(), dpow2.back()));
      pow2.push_back(add(pow2.back(), pow2.back()));
    }
    dpow2.pop_back();
    pow2.pop_back();

    // Calculate quotient by repeatedly subtracting from a.
    rem = a;
    auto dpiter = dpow2.rbegin();
    auto piter = pow2.rbegin();
    while (dpiter != dpow2.rend() && !rem.empty()) {
      if (compare(rem, *dpiter) >= 0) {
        isubtract(rem, *dpiter);
        iadd(quo, *piter);
      }
      ++dpiter;
      ++piter;
    }

    return std::make_pair(std::move(quo), std::move(rem));
  }
};


std::ostream &operator<<(std::ostream &out, const BigInt &b) {
  b.write(out);
  return out;
}

#endif//bigint_hh
