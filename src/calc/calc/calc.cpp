#include <calc/calc.hpp>

#include <debug/debug.hpp>

#include <cmath>

namespace jhmnn {

bool is_operand(const Token &t) {
  return t.type == Token::Type::Number || t.type == Token::Type::Identifier;
}

bool is_post_func(const Token &t) {
  switch (t.type) {
  case Token::Type::Factorial:
    return true;
  default:
    return false;
  };
}

bool is_pref_func(const Token &t) {
  switch (t.type) {
  case Token::Type::Negative:
  case Token::Type::Sqrt:
  case Token::Type::Sin:
  case Token::Type::Cos:
  case Token::Type::Tan:
  case Token::Type::Cot:
  case Token::Type::Log:
  case Token::Type::Log2:
  case Token::Type::Log10:
    return true;
  default:
    return false;
  };
}

bool is_func(const Token &t) { return is_pref_func(t) || is_post_func(t); }

bool is_bin_op(const Token &t) {
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

int op_prior(const Token &t) {
  if (is_pref_func(t)) {
    return 100;
  }

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
  default:
    return -1;
  }
}

bool is_left_associative(const Token & /*t*/) { return false; }

void Calc::calc_func(const Token &t) {
  const auto a = result_.top();
  result_.pop();

  switch (t.type) {
  case Token::Type::Negative:
    result_.push(-a);
    Debug::log("-%f\n", a);
    break;
  case Token::Type::Sqrt:
    result_.push(a.sqrt());
    Debug::log("sqrt(%f)\n", a);
    break;
  case Token::Type::Sin:
    result_.push(a.sin());
    Debug::log("sin(%f)\n", a);
    break;
  case Token::Type::Cos:
    result_.push(a.cos());
    Debug::log("cos(%f)\n", a);
    break;
  case Token::Type::Tan:
    result_.push(a.tan());
    Debug::log("tan(%f)\n", a);
    break;
  case Token::Type::Cot:
    result_.push(a.cot());
    Debug::log("cot(%f)\n", a);
    break;
  case Token::Type::Log:
    result_.push(a.log());
    Debug::log("log(%f)\n", a);
    break;
  case Token::Type::Log2:
    result_.push(a.log2());
    Debug::log("log2(%f)\n", a);
    break;
  case Token::Type::Log10:
    result_.push(a.log10());
    Debug::log("log10(%f)\n", a);
    break;
  case Token::Type::Factorial:
    result_.push(a.fac());
    Debug::log("fac(%f)\n", a);
    break;
  default:
    throw std::invalid_argument("Unknown function");
  }
}

void Calc::calc_bin_op(const Token &t) {
  const auto b = result_.top();
  result_.pop();
  const auto a = result_.top();
  result_.pop();

  switch (t.type) {
  case Token::Type::Plus:
    result_.push(a + b);
    Debug::log("%f + %f\n", a.value(), b.value());
    break;
  case Token::Type::Minus:
    result_.push(a - b);
    Debug::log("%f - %f\n", a.value(), b.value());
    break;
  case Token::Type::Mul:
    result_.push(a * b);
    Debug::log("%f * %f\n", a.value(), b.value());
    break;
  case Token::Type::Div:
    result_.push(a / b);
    Debug::log("%f / %f\n", a.value(), b.value());
    break;
  case Token::Type::Mod:
    result_.push(a % b);
    Debug::log("%f %% %f\n", a.value(), b.value());
    break;
  case Token::Type::Pow:
    result_.push(a.pow(b));
    Debug::log("%f ^ %f\n", a.value(), b.value());
    break;
  default:
    errors_.push_back("Unknown operation");
    break;
  }
}

std::vector<Token> expr_to_rpn(Lexer &lexer) {
  std::stack<Token> ops;
  std::vector<Token> out;

  lexer.first();
  while (lexer.more()) {
    auto token = lexer.next();
    if (is_operand(token) || is_post_func(token)) {
      out.push_back(token);
    } else if (is_pref_func(token) || token.type == Token::Type::RegOpen) {
      ops.push(token);
    } else if (token.type == Token::Type::RegClose) {
      while (!ops.empty() && ops.top().type != Token::Type::RegOpen) {
        out.push_back(ops.top());
        ops.pop();
      }
      ops.pop();
      if (!ops.empty()) {
        if (is_func(ops.top())) {
          out.push_back(ops.top());
          ops.pop();
        }
      }
    } else if (is_bin_op(token)) {
      while (!ops.empty()) {
        auto &top = ops.top();
        if (op_prior(top) >= op_prior(token)) {
          out.push_back(top);
          ops.pop();
        } else {
          break;
        }
      }

      ops.push(token);
    } else if (token.type == Token::Type::Unknown) {
      throw std::invalid_argument("'" + token.text + "' is unknown");
    }
  }

  while (!ops.empty()) {
    out.push_back(ops.top());
    ops.pop();
  }

  return out;
}

void Calc::calc_expr_rpn(std::vector<Token> &expr_rpn) {
  for (auto &token : expr_rpn) {
    if (is_operand(token)) {
      result_.push(std::strtod(token.text.c_str(), nullptr));
    } else if (is_func(token)) {
      calc_func(token);
    } else if (is_bin_op(token)) {
      calc_bin_op(token);
    }
  }

  if (result_.size() > 1) {
    throw std::runtime_error("Missing operator");
  }
}

double Calc::solve(Lexer &lexer) {
  while (!result_.empty()) {
    result_.pop();
  }

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

  return result_.top().value();
}

} // namespace jhmnn
