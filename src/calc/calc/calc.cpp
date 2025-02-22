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
  case Token::Type::Assign:
    return true;
  default:
    return false;
  }
}

bool is_left_associative(const Token &t) {
  switch (t.type) {
  case Token::Type::Plus:
  case Token::Type::Minus:
  case Token::Type::Mul:
  case Token::Type::Div:
  case Token::Type::Mod:
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
  case Token::Type::Assign:
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

std::vector<Token> expr_to_rpn(Lexer &lexer) {
  std::size_t reg_par_level = 0;

  std::stack<Token> ops;
  std::vector<Token> out;

  auto top_to_out = [&ops, &out]() {
    out.push_back(ops.top());
    ops.pop();
  };

  lexer.first();
  while (lexer.more()) {
    auto t = lexer.next();
    if (is_operand(t) || is_post_func(t)) {
      out.push_back(t);
    } else if (is_pref_func(t)) {
      ops.push(t);
    } else if (t.type == Token::Type::RegOpen) {
      ops.push(t);
      ++reg_par_level;
    } else if (t.type == Token::Type::RegClose) {
      while (true) {
        if (ops.empty()) {
          throw std::logic_error("Missing opening parenthesis");
        }

        if (ops.top().type == Token::Type::RegOpen) {
          break;
        }

        top_to_out();
      }

      --reg_par_level;
      ops.pop();
      if (!ops.empty() && is_pref_func(ops.top())) {
        top_to_out();
      }
    } else if (is_bin_op(t)) {
      while (!ops.empty()) {
        auto &top = ops.top();
        if (is_pref_func(top) ||
            (is_left_associative(top) && op_prior(top) >= op_prior(t))) {
          top_to_out();
        } else {
          break;
        }
      }
      ops.push(t);
    } else if (t.type == Token::Type::Unknown) {
      throw std::logic_error("'" + t.text + "' is unknown");
    }
  }

  while (!ops.empty()) {
    top_to_out();
  }

  if (reg_par_level > 0) {
    throw std::logic_error("Missing closing parenthesis");
  }

  return out;
}

Calc::Operand Calc::result_pop_l() {
  if (result_.empty()) {
    throw std::logic_error("Missing expression");
  }

  auto tmp = result_.top();
  result_.pop();
  if (tmp.type() == Token::Type::Number) {
    throw std::logic_error("Illegal assignment");
  }

  return tmp;
}

Calc::Operand Calc::result_pop_r() {
  if (result_.empty()) {
    throw std::logic_error("Missing expression");
  }

  auto tmp = result_.top();
  result_.pop();
  if (tmp.type() == Token::Type::Number) {
    return tmp;
  }

  try {
    return Operand(tmp_vars_.at(tmp.name()));
  } catch (const std::exception &e) {
    try {
      return Operand(vars_.at(tmp.name()));
    } catch (const std::exception &e) {
      throw std::logic_error("'" + tmp.name() + "' is undefined");
    }
  }
}

void Calc::calc_func(const Token &t) {
  const Rational &a = result_pop_r().value();

  switch (t.type) {
  case Token::Type::Negative:
    result_.push(-a);
    break;
  case Token::Type::Sqrt:
    result_.push(a.sqrt());
    break;
  case Token::Type::Sin:
    result_.push(a.sin());
    break;
  case Token::Type::Cos:
    result_.push(a.cos());
    break;
  case Token::Type::Tan:
    result_.push(a.tan());
    break;
  case Token::Type::Cot:
    result_.push(a.cot());
    break;
  case Token::Type::Log:
    result_.push(a.log());
    break;
  case Token::Type::Log2:
    result_.push(a.log2());
    break;
  case Token::Type::Log10:
    result_.push(a.log10());
    break;
  case Token::Type::Factorial:
    result_.push(a.fac());
    break;
  default:
    throw std::logic_error("Unknown function");
  }
}

void Calc::calc_bin_op(const Token &t) {
  const Rational &b = result_pop_r().value();
  const Rational &a = result_pop_r().value();

  switch (t.type) {
  case Token::Type::Plus:
    result_.push(a + b);
    break;
  case Token::Type::Minus:
    result_.push(a - b);
    break;
  case Token::Type::Mul:
    result_.push(a * b);
    break;
  case Token::Type::Div:
    result_.push(a / b);
    break;
  case Token::Type::Mod:
    result_.push(a % b);
    break;
  case Token::Type::Pow:
    result_.push(a.pow(b));
    break;
  default:
    throw std::logic_error("Unknown operation");
    break;
  }
}

void Calc::assign() {
  const Operand &r = result_pop_r();
  const Operand &l = result_pop_l();
  tmp_vars_[l.name()] = r.value();
  result_.push(l);
}

void Calc::calc_expr_rpn(std::vector<Token> &expr_rpn) {
  for (auto &t : expr_rpn) {
    if (t.type == Token::Type::Number) {
      result_.push(std::strtod(t.text.c_str(), nullptr));
    } else if (t.type == Token::Type::Identifier) {
      result_.push(t.text);
    } else if (t.type == Token::Type::Assign) {
      assign();
    } else if (is_func(t)) {
      calc_func(t);
    } else if (is_bin_op(t)) {
      calc_bin_op(t);
    }
  }

  if (result_.size() > 1) {
    throw std::logic_error("Missing operator");
  }

  if (result_.top().type() == Token::Type::Identifier) {
    result_.push(result_pop_r());
  }
}

double Calc::solve(Lexer &lexer) {
  is_correct_ = false;
  tmp_vars_ = vars_;
  while (!result_.empty()) {
    result_.pop();
  }

  auto expr_rpn = expr_to_rpn(lexer);
  if (expr_rpn.empty()) {
    return 0;
  }

  calc_expr_rpn(expr_rpn);
  is_correct_ = true;
  return result_pop_r().value().value();
}

void Calc::save_vars() { vars_ = tmp_vars_; }

bool Calc::is_correct() const { return is_correct_; }

} // namespace jhmnn
