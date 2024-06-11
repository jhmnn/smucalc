#pragma once

#include <lexer/lexer.hpp>
#include <rational/rational.hpp>

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace jhmnn {

class Calc {
public:
  double solve(Lexer &lexer);

private:
  struct Operand {
  public:
    Operand(const std::string &name)
        : name_(name), value_(0), type_(Token::Type::Identifier) {}

    Operand(const Rational &value)
        : value_(value), type_(Token::Type::Number) {}

    Operand(double value) : value_(value), type_(Token::Type::Number) {}

    std::string name() const { return name_; }
    Rational value() const { return value_; }
    Token::Type type() const { return type_; }

  private:
    const std::string name_;
    Rational value_;
    Token::Type type_;
  };

private:
  Operand result_pop_l();
  Operand result_pop_r();

  void assign();
  void calc_func(const Token &t);
  void calc_bin_op(const Token &t);
  void calc_expr_rpn(std::vector<Token> &expr_rpn);

private:
  std::unordered_map<std::string, Rational> vars_;

  std::stack<Operand> result_;
};

} // namespace jhmnn
