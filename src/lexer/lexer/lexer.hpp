#pragma once

#include <string>
#include <vector>

namespace jhmnn {

struct Token {
public:
  enum class Type {
    Number,
    Identifier,
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
    Pow,
    Negative,
    Factorial,
    Sqrt,
    Sin,
    Cos,
    Tan,
    Cot,
    Log,
    Log2,
    Log10,
    RegOpen,
    RegClose,
    Unknown
  };

public:
  Token() : type(Type::Unknown) {}
  Token(const std::string &text_, Type type_) : text(text_), type(type_) {}
  Token(char text_, Type type_) : type(type_) { text += text_; }

public:
  std::string text;
  Type type;
};

class Lexer {
public:
  void parse(const std::string &str);

  void first();
  Token next();
  bool more() const;

private:
  Token make_token_identifier();
  Token make_token_number();

private:
  std::string::const_iterator it_;

  std::vector<Token> result_;
  std::size_t current_ = 0;
};

} // namespace jhmnn
