#include <lexer/lexer.hpp>

namespace jhmnn {

bool is_number(char c) { return std::isdigit(c) || c == '.'; }

bool is_identifier(char c) { return std::isalpha(c) || c == '_'; }

void change_if_function(Token &t) {
  const std::string &s = t.text;
  if (s == "sqrt") {
    t.type = Token::Type::Sqrt;
  } else if (s == "sin") {
    t.type = Token::Type::Sin;
  } else if (s == "cos") {
    t.type = Token::Type::Cos;
  } else if (s == "tan") {
    t.type = Token::Type::Tan;
  } else if (s == "cot") {
    t.type = Token::Type::Cot;
  } else if (s == "ln") {
    t.type = Token::Type::Log;
  } else if (s == "log") {
    t.type = Token::Type::Log2;
  } else if (s == "lg") {
    t.type = Token::Type::Log10;
  }
}

Token::Type define_minus_type(const std::vector<Token> &v) {
  if (v.empty()) {
    return Token::Type::Negative;
  }

  switch (v.back().type) {
  case Token::Type::Number:
  case Token::Type::Identifier:
  case Token::Type::RegClose:
    return Token::Type::Minus;
  default:
    return Token::Type::Negative;
  }
}

Token Lexer::make_token_identifier() {
  std::string token;
  while (is_identifier(*it_)) {
    token += *it_++;
  }

  Token t = {token, Token::Type::Identifier};
  change_if_function(t);

  return t;
}

Token Lexer::make_token_number() {
  std::string token;
  while (is_number(*it_)) {
    token += *it_++;
  }

  return {token, Token::Type::Number};
}

void Lexer::parse(const std::string &str) {
  result_.clear();
  it_ = str.begin();

  while (it_ != str.end()) {
    while (isspace(*it_)) {
      if (++it_ == str.end()) {
        return;
      }
    }

    if (is_number(*it_)) {
      result_.push_back(make_token_number());
      continue;
    }

    if (is_identifier(*it_)) {
      result_.push_back(make_token_identifier());
      continue;
    }

    switch (*it_) {
    case '+':
      result_.push_back({*it_, Token::Type::Plus});
      break;
    case '-':
      result_.push_back({*it_, define_minus_type(result_)});
      break;
    case '*':
      result_.push_back({*it_, Token::Type::Mul});
      break;
    case '/':
      result_.push_back({*it_, Token::Type::Div});
      break;
    case '%':
      result_.push_back({*it_, Token::Type::Mod});
      break;
    case '^':
      result_.push_back({*it_, Token::Type::Pow});
      break;
    case '!':
      result_.push_back({*it_, Token::Type::Factorial});
      break;
    case '(':
      result_.push_back({*it_, Token::Type::RegOpen});
      break;
    case ')':
      result_.push_back({*it_, Token::Type::RegClose});
      break;
    default:
      result_.push_back({*it_, Token::Type::Unknown});
      break;
    }

    ++it_;
  }

  current_ = 0;
}

void Lexer::first() { current_ = 0; }

Token Lexer::next() {
  if (more()) {
    return result_[current_++];
  }
  return {"", Token::Type::Unknown};
}

bool Lexer::more() const { return current_ < result_.size(); }

} // namespace jhmnn
