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
  void calc_expr_rpn(std::vector<Token> &expr_rpn);
  void calc_func(const Token &t);
  void calc_bin_op(const Token &t);
  void append_operand(const Token &t);

private:
  std::unordered_map<std::string, Rational> vars_;
  std::stack<Rational> result_;
};

} // namespace jhmnn
