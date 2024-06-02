#pragma once

#include <lexer/lexer.hpp>
#include <rational/rational.hpp>

#include <stack>
#include <string>
#include <vector>

namespace jhmnn {

class Calc {
public:
  double solve(Lexer &lexer);

private:
  void calc_expr_rpn(std::vector<Token> &expr_rpn);
  void calc_func(const Token &t);
  void calc_bin_op(const Token &t);

private:
  std::stack<Rational> result_;
  std::vector<std::string> errors_;
};

} // namespace jhmnn
