#include <lexer/lexer.hpp>

namespace jhmnn {

bool is_number(char c) { return std::isdigit(c) || c == '.'; }

bool is_identifier(char c) { return std::isalpha(c) || c == '_'; }

Token::Type define_minus_type(const Token &t) {
  switch (t.type) {
  case Token::Number:
  case Token::Identifier:
  case Token::RegClose:
    return Token::Minus;
  default:
    return Token::Negative;
  }
}

Token Lexer::make_token_identifier() {
  std::string token;
  while (is_identifier(*it_)) {
    token += *it_++;
  }
  return {token, Token::Identifier};
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
      result_.push_back({*it_, Token::Plus});
      break;
    case '-':
      result_.push_back({*it_, define_minus_type(result_.back())});
      break;
    case '*':
      result_.push_back({*it_, Token::Mul});
      break;
    case '/':
      result_.push_back({*it_, Token::Div});
      break;
    case '%':
      result_.push_back({*it_, Token::Mod});
      break;
    case '(':
      result_.push_back({*it_, Token::RegOpen});
      break;
    case ')':
      result_.push_back({*it_, Token::RegClose});
      break;
    default:
      result_.push_back({*it_, Token::Unknown});
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
  return {"", Token::Unknown};
}

bool Lexer::more() const { return current_ < result_.size(); }

} // namespace jhmnn
