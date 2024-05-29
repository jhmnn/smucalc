#include <syntaxer/syntaxer.hpp>

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
  case Token::Plus:
  case Token::Minus:
    return 0;
  case Token::Mul:
  case Token::Div:
  case Token::Mod:
    return 1;
  default:
    return -1;
  }
}

bool is_left_associative(const Token & /*t*/) { return false; }

void Syntaxer::calc_function(const Token & /*t*/) {}

void Syntaxer::calc_binary_operation(const Token &t) {
  const double a = result_.top();
  result_.pop();
  const double b = result_.top();
  result_.pop();

  switch (t.type) {
  case Token::Plus:
    result_.push(a + b);
    break;

  case Token::Minus:
    result_.push(a - b);
    break;

  case Token::Mul:
    result_.push(a * b);
    break;

  case Token::Div:
    result_.push(a / b);
    break;

  case Token::Mod:
    result_.push(static_cast<int>(a) % static_cast<int>(b));
    break;

  default:
    // Check for errors
    break;
  }
}

std::vector<Token *> expr_to_rpn(Lexer &lexer) {
  std::stack<Token *> operations;
  std::vector<Token *> out;

  lexer.first();
  while (lexer.more()) {
    auto token = lexer.next();
    if (is_number(token) || is_postfix_function(token)) {
      out.push_back(&token);
    } else if (is_prefix_function(token) || token.type == Token::RegOpen) {
      operations.push(&token);
    } else if (token.type == Token::RegClose) {
      while (operations.top()->type != Token::RegOpen) {
        out.push_back(operations.top());
        operations.pop();
        // ERROR HANDLING (empty stack)
      }
      operations.pop();
    } else if (is_binary_operation(token)) {
      while (!operations.empty()) {
        auto &top = *operations.top();
        if (operation_priority(top) < operation_priority(token)) {
          break;
        }
        if (!(is_left_associative(top) &&
              operation_priority(top) == operation_priority(token)) &&
            !is_prefix_function(top)) {
          break;
        }

        out.push_back(&top);
        operations.pop();
      }

      operations.push(&token);
    }
  }

  while (!operations.empty()) {
    out.push_back(operations.top());
    operations.pop();
  }

  return out;
}

void Syntaxer::calc_expression(std::vector<Token *> &expr_rpn) {
  for (auto &i : expr_rpn) {
    auto &token = *i;
    if (is_number(token)) {
      result_.push(std::strtof(token.text.c_str(), nullptr));
    } else if (is_function(token)) {
      calc_function(token);
    } else if (is_binary_operation(token)) {
      calc_binary_operation(token);
    }
  }
}

double Syntaxer::calc_expr(Lexer &lexer) {
  auto expr_rpn = expr_to_rpn(lexer);
  calc_expression(expr_rpn);
  return result_.top();
}

} // namespace jhmnn
