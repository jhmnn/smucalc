#include <calc/calc.hpp>

#include <debug/debug.hpp>

namespace jhmnn {

bool is_number(const Token &t) {
  switch (t.type) {
  case Token::Int:
  case Token::Float:
  case Token::Identifier:
    return true;
  default:
    return false;
  }
}

bool is_postfix_function(const Token & /*t*/) { return false; }

bool is_prefix_function(const Token & /*t*/) { return false; }

bool is_function(const Token &t) {
  return is_prefix_function(t) || is_postfix_function(t);
}

bool is_binary_operation(const Token &t) {
  switch (t.type) {
  case Token::Plus:
  case Token::Minus:
  case Token::Mul:
  case Token::Div:
  case Token::Mod:
    return true;
  default:
    return false;
  }
}

int operation_priority(const Token &t) {
  switch (t.type) {
  case Token::RegOpen:
    return 0;
  case Token::Plus:
  case Token::Minus:
    return 1;
  case Token::Mul:
  case Token::Div:
  case Token::Mod:
    return 2;
  default:
    return -1;
  }
}

bool is_left_associative(const Token & /*t*/) { return false; }

void Calc::calc_function(const Token & /*t*/) {}

void Calc::calc_binary_operation(const Token &t) {
  const double b = result_.top();
  result_.pop();
  const double a = result_.top();
  result_.pop();

  switch (t.type) {
  case Token::Plus:
    result_.push(a + b);
    Debug::log("%f + %f\n", a, b);
    break;

  case Token::Minus:
    result_.push(a - b);
    Debug::log("%f - %f\n", a, b);
    break;

  case Token::Mul:
    result_.push(a * b);
    Debug::log("%f * %f\n", a, b);
    break;

  case Token::Div:
    result_.push(a / b);
    Debug::log("%f / %f\n", a, b);
    break;

  case Token::Mod:
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
    if (is_number(token) || is_postfix_function(token)) {
      out.push_back(token);
    } else if (is_prefix_function(token) || token.type == Token::RegOpen) {
      operations.push(token);
    } else if (token.type == Token::RegClose) {
      while (!operations.empty() && operations.top().type != Token::RegOpen) {
        out.push_back(operations.top());
        operations.pop();
      }
      operations.pop();
    } else if (is_binary_operation(token)) {
      while (!operations.empty()) {
        auto &top = operations.top();
        if (operation_priority(top) >= operation_priority(token) ||
            (is_left_associative(top) &&
             operation_priority(top) == operation_priority(token)) ||
            is_prefix_function(top)) {
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
    if (is_number(token)) {
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
