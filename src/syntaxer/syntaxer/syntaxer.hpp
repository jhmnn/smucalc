#pragma once

#include <lexer/lexer.hpp>

#include <stack>
#include <string>
#include <vector>

namespace jhmnn {

class Syntaxer {
public:
  double calc_expr(Lexer &lexer);

private:
  void calc_expression(std::vector<Token *> &expr_rpn);
  void calc_function(const Token &t);
  void calc_binary_operation(const Token &t);

private:
  std::stack<double> result_;
  std::vector<std::string> errors_;
};

} // namespace jhmnn
