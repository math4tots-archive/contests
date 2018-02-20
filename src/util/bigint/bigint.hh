#ifndef bigint_hh
#define bigint_hh

// Requires C++11

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct BigInt final {
  using Digit = long;
  using Digit2 = long long;
  using Sign = bool;
  using Digits = std::vector<Digit>;

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
    remove_extraneous_zeros();
  }
  explicit BigInt(Sign s, Digits dd): sign(s), digits(std::move(dd)) {
    remove_extraneous_zeros();
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
      return other.digits < digits;
    }
    return digits < other.digits;
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
    if (sign == b.sign) {
      return BigInt(sign, add(digits, b.digits));
    }
    if (sign == PLUS) {
      return subtract(digits, b.digits);
    }
    return subtract(b.digits, digits);
  }

  BigInt operator*(const BigInt &b) const {
    // TODO: Use better algorithm than long multiplication
    std::vector<Digit> dd(digits.size() + b.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < b.digits.size(); j++) {
        auto digit = static_cast<Digit2>(digits[i]) *
                     static_cast<Digit2>(b.digits[j]);
        auto pos = i + j;
        while (digit) {
          dd[pos++] += digit % BASE;
          digit /= BASE;
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

  BigInt shift(size_t n) const {
    Digits dd;
    for (size_t i = 0; i < n; i++) {
      dd.push_back(0);
    }
    for (auto digit: digits) {
      dd.push_back(digit);
    }
    return BigInt(sign, std::move(dd));
  }

  void remove_extraneous_zeros() {
    while (!digits.empty() && digits.back() == 0) {
      digits.pop_back();
    }
  }

  void flip_sign() {
    sign = !sign;
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

    while (!a.empty() && a.back() == 0) {
      a.pop_back();
    }
  }

  static Digits add(const Digits &a, const Digits &b) {
    Digit2 carry = 0;
    Digits ret;
    size_t i = 0;

    while (i < a.size() || i < b.size() || carry) {
      Digit2 aval = static_cast<Digit2>(i < a.size() ? a[i] : 0);
      Digit2 bval = static_cast<Digit2>(i < b.size() ? b[i] : 0);
      Digit2 next = aval + bval + carry;
      ret.push_back(next % BASE);
      carry = next / BASE;
      i++;
    }

    while (!ret.empty() && ret.back() == 0) {
      ret.pop_back();
    }

    return ret;
  }

  static std::pair<Digit, Digits>
  subtract_helper(const Digits &a, const Digits &b) {
    Digit borrow = 0;
    Digits dd;
    Sign sign = PLUS;
    size_t i = 0;

    while (i < a.size() || i < b.size()) {
      Digit aval = i < a.size() ? a[i] : 0;
      Digit bval = i < b.size() ? b[i] : 0;
      Digit next = aval - bval - borrow;
      borrow = 0;
      while (next < 0) {
        next += BASE;
        borrow++;
      }
      dd.push_back(next);
      i++;
    }

    return std::make_pair(sign, std::move(dd));
  }

  static BigInt subtract(const Digits &a, const Digits &b) {
    auto pair1 = subtract_helper(a, b);
    if (pair1.first == 0) {
      return BigInt(PLUS, std::move(pair1.second));
    }
    auto pair2 = subtract_helper(b, a);
    return BigInt(MINUS, std::move(pair2.second));
  }
};


std::ostream &operator<<(std::ostream &out, const BigInt &b) {
  b.write(out);
  return out;
}

#endif//bigint_hh
