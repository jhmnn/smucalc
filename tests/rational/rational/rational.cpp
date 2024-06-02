#include <gtest/gtest.h>

#include <rational/rational.hpp>

#include <cmath>

TEST(Rational, Constructor1) {
  const jhmnn::Rational r1(5);
  EXPECT_EQ(r1.value(), 5);
  const jhmnn::Rational r2 = -5;
  EXPECT_EQ(r2.value(), -5);
}

TEST(Rational, Constructor2) {
  const jhmnn::Rational r1(5, 1);
  EXPECT_EQ(r1.value(), 5);
  const jhmnn::Rational r2(5, -1);
  EXPECT_EQ(r2.value(), -5);
  const jhmnn::Rational r3(-5, 1);
  EXPECT_EQ(r3.value(), -5);
  const jhmnn::Rational r4 = {5, 2};
  EXPECT_EQ(r4.value(), 2.5);

  EXPECT_ANY_THROW(const jhmnn::Rational r5(5, 0));
}

TEST(Rational, ConstructorDouble) {
  double i = -142134;
  while (i < 100055.512) {
    const jhmnn::Rational r(i);
    EXPECT_EQ(r.value(), i);
    i += 15141.52;
  }
}

TEST(Rational, OperatorEqual) {
  const jhmnn::Rational r1(5, 2);
  const jhmnn::Rational r2(r1);
  EXPECT_TRUE(r1 == r2);
}

TEST(Rational, OperatorNotEqual) {
  const jhmnn::Rational r1(5, 2);
  const jhmnn::Rational r2(r1);
  EXPECT_FALSE(r1 != r2);
}

TEST(Rational, OperatorGreater) {
  const jhmnn::Rational r1(5, 2);
  const jhmnn::Rational r2(1, 2);
  EXPECT_TRUE(r1 > r2);
}

TEST(Rational, OperatorLess) {
  const jhmnn::Rational r1(-5);
  const jhmnn::Rational r2(5, 2);
  EXPECT_TRUE(r1 < r2);
}

TEST(Rational, OperatorGreaterOrEqual) {
  const jhmnn::Rational r1(5, 2);
  jhmnn::Rational r2(1, 2);
  EXPECT_TRUE(r1 >= r2);

  r2 = r1;
  EXPECT_TRUE(r1 >= r2);
}

TEST(Rational, OperatorLessOrEqual) {
  const jhmnn::Rational r1(-1, 2);
  jhmnn::Rational r2(-1, 6);
  EXPECT_TRUE(r1 <= r2);

  r2 = r1;
  EXPECT_TRUE(r1 <= r2);
}

TEST(Rational, OperatorPlus) {
  jhmnn::Rational r1(1, 2);
  const jhmnn::Rational r2(1, 2);
  EXPECT_EQ((r1 + r2).value(), 1.0);

  r1 = {-1, 2};
  EXPECT_EQ((r1 + r2).value(), 0.0);
}

TEST(Rational, OperatorMinus) {
  jhmnn::Rational r1(1, 1);
  jhmnn::Rational r2(1, 2);
  EXPECT_EQ((r1 - r2).value(), 0.5);

  r1 = {0};
  r2 = {1, 2};
  EXPECT_EQ((r1 - r2).value(), -0.5);
}

TEST(Rational, OperatorMultiply) {
  jhmnn::Rational r1(2, 3);
  jhmnn::Rational r2(-3, 2);
  EXPECT_EQ((r1 * r2).value(), -1);

  r1 = {-1, 2};
  r2 = {1, -2};
  EXPECT_EQ((r1 * r2).value(), 0.25);
}

TEST(Rational, OperatorDivide) {
  jhmnn::Rational r1(5, 6);
  jhmnn::Rational r2(5, 6);
  EXPECT_EQ((r1 / r2).value(), 1);

  r1 = {-1, 2};
  r2 = {2, -1};
  EXPECT_EQ((r1 / r2).value(), 0.25);
}

TEST(Rational, OperatorNegative) {
  jhmnn::Rational r1(1, 8);

  r1 = -r1;
  EXPECT_EQ(r1.value(), -0.125);
}

TEST(Rational, Value) {
  jhmnn::Rational r1(1, 8);
  EXPECT_EQ(r1.value(), 0.125);

  r1 = {-2, 8};
  EXPECT_EQ(r1.value(), -0.25);
}

TEST(Rational, Fac) {
  jhmnn::Rational r1(8, 1);
  r1 = r1.fac();
  EXPECT_EQ(r1.value(), 40320.0);
}

TEST(Rational, Pow) {
  jhmnn::Rational r1(8, 4);

  r1 = r1.pow(2);
  EXPECT_EQ(r1.value(), 4.0);

  r1 = r1.pow(2.5);
  EXPECT_EQ(r1.value(), 32.0);

  r1 = r1.pow(-1);
  EXPECT_EQ(r1.value(), 0.03125);
}

TEST(Rational, Sqr) {
  jhmnn::Rational r1(8, 2);
  r1 = r1.sqr();
  EXPECT_EQ(r1.value(), 16.0);
}

TEST(Rational, Sqrt) {
  jhmnn::Rational r1(82, 54);
  r1 = r1.sqrt();
  EXPECT_EQ(r1.value(), std::sqrt(82.0 / 54));
}

TEST(Rational, Sin) {
  jhmnn::Rational r1(73, 61);
  r1 = r1.sin();
  EXPECT_EQ(r1.value(), std::sin(73.0 / 61));
}

TEST(Rational, Cos) {
  jhmnn::Rational r1(99, 55);
  r1 = r1.cos();
  EXPECT_EQ(r1.value(), std::cos(99.0 / 55));
}

TEST(Rational, Tan) {
  jhmnn::Rational r1(324, 515);
  r1 = r1.tan();
  EXPECT_EQ(r1.value(), std::tan(324.0 / 515));
}

TEST(Rational, Cot) {
  jhmnn::Rational r1(324, 515);
  r1 = r1.cot();
  EXPECT_EQ(r1.value(), 1.0 / std::tan(324.0 / 515));
}

TEST(Rational, Log) {
  jhmnn::Rational r1(123, 456);
  r1 = r1.log();
  EXPECT_EQ(r1.value(), std::log(123.0 / 456));
}

TEST(Rational, Log2) {
  jhmnn::Rational r1(123, 456);
  r1 = r1.log2();
  EXPECT_EQ(r1.value(), std::log2(123.0 / 456));
}

TEST(Rational, Log10) {
  jhmnn::Rational r1(262, 456);
  r1 = r1.log10();
  EXPECT_EQ(r1.value(), std::log10(262.0 / 456));
}
