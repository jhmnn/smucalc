#include <calc/calc.hpp>

#include <debug/debug.hpp>

#include <cmath>

namespace jhmnn {

bool is_operand(const Token &t) {
  return t.type == Token::Type::Number || t.type == Token::Type::Identifier;
}

bool is_postfix_function(const Token &t) {
  switch (t.type) {
  case Token::Type::Factorial:
    return true;
  default:
    return false;
  };
}

bool is_prefix_function(const Token &t) {
  switch (t.type) {
  case Token::Type::Negative:
  case Token::Type::Sin:
  case Token::Type::Cos:
  case Token::Type::Tan:
    return true;
  default:
    return false;
  };
}

bool is_function(const Token &t) {
  return is_prefix_function(t) || is_postfix_function(t);
}

bool is_binary_operation(const Token &t) {
  switch (t.type) {
  case Token::Type::Plus:
  case Token::Type::Minus:
  case Token::Type::Mul:
  case Token::Type::Div:
  case Token::Type::Mod:
  case Token::Type::Pow:
    return true;
  default:
    return false;
  }
}

int operation_priority(const Token &t) {
  switch (t.type) {
  case Token::Type::RegOpen:
    return 0;
  case Token::Type::Plus:
  case Token::Type::Minus:
    return 1;
  case Token::Type::Mul:
  case Token::Type::Div:
  case Token::Type::Mod:
    return 2;
  case Token::Type::Pow:
    return 3;
  case Token::Type::Negative:
  case Token::Type::Sin:
  case Token::Type::Cos:
  case Token::Type::Tan:
    return 4;
  default:
    return -1;
  }
}

bool is_left_associative(const Token & /*t*/) { return false; }

int factorial(int n) {
  for (auto i = 0; i < n; ++i) {
    n *= i;
  }
  return n;
}

void Calc::calc_function(const Token & /*t*/) {
  // const double a = result_.top();
  // result_.pop();

  // switch (t.type) {
  // case Token::Type::Negative:
  //   result_.push(-a);
  //   Debug::log("-%f\n", a);
  // case Token::Type::Sin:
  //   result_.push(std::sin(a));
  //   Debug::log("sin(%f)\n", a);
  // case Token::Type::Cos:
  //   result_.push(std::cos(a));
  //   Debug::log("cos(%f)\n", a);
  // case Token::Type::Tan:
  //   result_.push(std::tan(a));
  //   Debug::log("tan(%f)\n", a);
  // case Token::Type::Factorial:
  //   result_.push(factorial(static_cast<int>(a)));
  //   Debug::log("cos(%f)\n", a);
  // }
}

void Calc::calc_binary_operation(const Token &t) {
  const double b = result_.top();
  result_.pop();
  const double a = result_.top();
  result_.pop();

  switch (t.type) {
  case Token::Type::Plus:
    result_.push(a + b);
    Debug::log("%f + %f\n", a, b);
    break;

  case Token::Type::Minus:
    result_.push(a - b);
    Debug::log("%f - %f\n", a, b);
    break;

  case Token::Type::Mul:
    result_.push(a * b);
    Debug::log("%f * %f\n", a, b);
    break;

  case Token::Type::Div:
    result_.push(a / b);
    Debug::log("%f / %f\n", a, b);
    break;

  case Token::Type::Mod:
    result_.push(static_cast<int>(a) % static_cast<int>(b));
    Debug::log("%f ^ %f\n", a, b);
    break;

  default:
    // Check for errors
    break;
  }
}

std::vector<Token> expr_to_rpn(Lexer &lexer) {
  std::stack<Token> operations;
  std::vector<Token> out;

  lexer.first();
  while (lexer.more()) {
    auto token = lexer.next();
    if (is_operand(token) || is_postfix_function(token)) {
      out.push_back(token);
    } else if (
        is_prefix_function(token) || token.type == Token::Type::RegOpen) {
      operations.push(token);
    } else if (token.type == Token::Type::RegClose) {
      while (!operations.empty() &&
             operations.top().type != Token::Type::RegOpen) {
        out.push_back(operations.top());
        operations.pop();
      }
      operations.pop();
    } else if (is_binary_operation(token)) {
      while (!operations.empty()) {
        auto &top = operations.top();
        if (operation_priority(top) >= operation_priority(token)) {
          out.push_back(top);
          operations.pop();
        } else {
          break;
        }
      }

      operations.push(token);
    }
  }

  while (!operations.empty()) {
    out.push_back(operations.top());
    operations.pop();
  }

  return out;
}

void Calc::calc_expr_rpn(std::vector<Token> &expr_rpn) {
  for (auto &token : expr_rpn) {
    if (is_operand(token)) {
      result_.push(std::strtof(token.text.c_str(), nullptr));
    } else if (is_function(token)) {
      calc_function(token);
    } else if (is_binary_operation(token)) {
      calc_binary_operation(token);
    }
  }
}

double Calc::solve(Lexer &lexer) {
  Debug::log("expr: ");
  lexer.first();
  while (lexer.more()) {
    auto t = lexer.next();
    Debug::log("%s ", t.text.c_str());
  }
  Debug::log("\n");

  auto expr_rpn = expr_to_rpn(lexer);

  Debug::log("expr rpn: ");
  for (const auto &i : expr_rpn) {
    Debug::log("%s ", i.text.c_str());
  }
  Debug::log("\n");

  calc_expr_rpn(expr_rpn);
  Debug::log("result: %f\n", result_.top());

  const double tmp = result_.top();
  result_.pop();

  return tmp;
}

} // namespace jhmnn
