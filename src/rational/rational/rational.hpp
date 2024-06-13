#pragma once

#include <cstdint>
#include <stdexcept>

namespace jhmnn {

class Rational {
public:
  Rational();
  Rational(double number);
  template <typename Int> Rational(Int number);
  template <typename Int1, typename Int2>
  Rational(Int1 numerator, Int2 denominator);

  Rational(const Rational &x) = default;
  Rational(Rational &&x) noexcept = default;

  ~Rational() = default;

  Rational &operator=(const Rational &x);
  Rational &operator=(Rational &&x) noexcept;

  friend bool operator==(const Rational &x, const Rational &y);
  friend bool operator!=(const Rational &x, const Rational &y);
  friend bool operator>(const Rational &x, const Rational &y);
  friend bool operator<=(const Rational &x, const Rational &y);
  friend bool operator<(const Rational &x, const Rational &y);
  friend bool operator>=(const Rational &x, const Rational &y);

  friend Rational operator+(const Rational &x, const Rational &y);
  friend Rational operator-(const Rational &x, const Rational &y);
  friend Rational operator*(const Rational &x, const Rational &y);
  friend Rational operator/(const Rational &x, const Rational &y);
  friend Rational operator%(const Rational &x, const Rational &y);

  Rational operator-() const;

  Rational &operator+=(const Rational &x);
  Rational &operator-=(const Rational &x);
  Rational &operator*=(const Rational &x);
  Rational &operator/=(const Rational &x);
  Rational &operator%=(const Rational &x);

  double value() const;
  int integ() const;

  double fac() const;
  double pow(const Rational &x) const;
  double sqr() const;
  double sqrt() const;
  double sin() const;
  double cos() const;
  double tan() const;
  double cot() const;
  double log() const;
  double log2() const;
  double log10() const;

private:
  void reduce();
  void convert(double number);

private:
  std::int64_t num_;
  std::int64_t den_;
};

template <typename Int>
Rational::Rational(Int number)
    : num_(static_cast<std::int64_t>(number)), den_(1) {}

template <typename Int1, typename Int2>
Rational::Rational(Int1 numerator, Int2 denominator)
    : num_{static_cast<std::int64_t>(numerator)}, den_{
                                                      static_cast<std::int64_t>(
                                                          denominator)} {
  if (den_ < 0) {
    num_ = -num_;
    den_ = -den_;
  } else if (den_ == 0) {
    throw std::invalid_argument("The denominator cannot be zero");
  }

  reduce();
}

} // namespace jhmnn
