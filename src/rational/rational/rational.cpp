#include <rational/rational.hpp>

#include <cmath>
#include <numeric>

namespace jhmnn {

struct FactTable {
  constexpr FactTable() {
    data[0] = 1;
    for (std::size_t i = 1; i < size; ++i) {
      data[i] = data[i - 1] * i;
    }
  }

  static constexpr std::size_t size = 20;
  std::uint64_t data[size]{0};
};

std::uint64_t apow2(int x) {
  return static_cast<std::uint64_t>(1) << (std::abs(x));
}

Rational::Rational() : num_{0}, den_{1} {}

Rational::Rational(double number) : num_{0}, den_{1} { convert(number); }

Rational &Rational::operator=(const Rational &x) {
  if (this != &x) {
    num_ = x.num_;
    den_ = x.den_;
  }
  return *this;
}

Rational &Rational::operator=(Rational &&x) noexcept {
  if (this != &x) {
    num_ = x.num_;
    den_ = x.den_;
  }
  return *this;
}

bool operator==(const Rational &x, const Rational &y) {
  return (x.num_ == y.num_) && (x.den_ == y.den_);
}

bool operator!=(const Rational &x, const Rational &y) { return !(x == y); }

bool operator>(const Rational &x, const Rational &y) {
  return (x.num_ * y.den_) > (y.num_ * x.den_);
}

bool operator<=(const Rational &x, const Rational &y) { return !(x > y); }

bool operator<(const Rational &x, const Rational &y) {
  return (x.num_ * y.den_) < (y.num_ * x.den_);
}

bool operator>=(const Rational &x, const Rational &y) { return !(x < y); }

Rational operator+(const Rational &x, const Rational &y) {
  auto den = std::max(x.den_, y.den_) *
      (std::min(x.den_, y.den_) / std::gcd(x.den_, y.den_));
  return {(x.num_ * (den / x.den_)) + (y.num_ * (den / y.den_)), den};
}

Rational operator-(const Rational &x, const Rational &y) { return -y + x; }

Rational operator*(const Rational &x, const Rational &y) {
  return {x.num_ * y.num_, x.den_ * y.den_};
}

Rational operator/(const Rational &x, const Rational &y) {
  return {x.num_ * y.den_, y.num_ * x.den_};
}

Rational operator%(const Rational &x, const Rational &y) {
  return x < y ? x : (x - (y * (x / y).integ()));
}

Rational Rational::operator-() const { return {-num_, den_}; }

Rational &Rational::operator+=(const Rational &x) {
  *this = *this + x;
  return *this;
}

Rational &Rational::operator-=(const Rational &x) {
  *this = *this - x;
  return *this;
}

Rational &Rational::operator*=(const Rational &x) {
  *this = *this * x;
  return *this;
}

Rational &Rational::operator/=(const Rational &x) {
  *this = *this / x;
  return *this;
}

Rational &Rational::operator%=(const Rational &x) {
  *this = *this % x;
  return *this;
}

void Rational::reduce() {
  auto x = std::gcd(num_, den_);
  num_ /= x;
  den_ /= x;
}

void Rational::convert(double number) {
  const std::size_t exp_size = 11;
  const std::size_t mant_size = 52;

  auto *p = reinterpret_cast<std::uint64_t *>(&number);
  auto bits = *p;
  const int exp = ((bits >> mant_size) & 0x7FF) - 1023;
  int i = exp - mant_size;

  Rational tmp = 0;
  for (; i < 0 && i < exp; ++i) {
    auto n = (bits & 0x1) * apow2(i);
    if (n > 0) {
      tmp += {1, n};
    }
    bits >>= 1;
  }
  for (; i < exp; ++i) {
    num_ += (bits & 0x1) * apow2(i);
    bits >>= 1;
  }

  if (exp >= 0) {
    num_ += apow2(exp);
  } else if (exp != -1023) {
    tmp += {1, apow2(exp)};
  }

  tmp += num_;
  num_ = tmp.num_;
  den_ = tmp.den_;
  num_ = (bits >> exp_size) == 1 ? -num_ : num_;
}

double Rational::value() const { return static_cast<double>(num_) / den_; }

int Rational::integ() const { return num_ / den_; }

double Rational::fac() const {
  if (num_ < 0) {
    throw std::runtime_error("Factorial argument is negative");
  }

  if (static_cast<std::size_t>(num_) > FactTable::size) {
    throw std::runtime_error("Factorial argument is too big");
  }

  if (den_ != 1) {
    throw std::runtime_error("Factorial argument is non-integral");
  }

  return static_cast<double>(FactTable().data[num_]);
}

double Rational::pow(const Rational &x) const {
  return std::pow(value(), x.value());
}

double Rational::sqr() const { return (value() * value()); }

double Rational::sqrt() const { return std::sqrt(value()); }

double Rational::sin() const { return std::sin(value()); }

double Rational::cos() const { return std::cos(value()); }

double Rational::tan() const { return std::tan(value()); }

double Rational::cot() const { return 1.0 / std::tan(value()); }

double Rational::log() const { return std::log(value()); }

double Rational::log2() const { return std::log2(value()); }

double Rational::log10() const { return std::log10(value()); }

} // namespace jhmnn
