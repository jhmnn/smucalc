#include <wrl/wrl.hpp>

#include <cstdio>

namespace jhmnn {

Wrl::Wrl(std::string &buffer) : buffer_(buffer) {}
Wrl::Wrl(std::string &buffer, const std::string &prefix)
    : prefix_(prefix), buffer_(buffer) {}
Wrl::~Wrl() { finalize(); }

void Wrl::init() { tic_.init(); }

void Wrl::finalize() { tic_.finalize(); }

void Wrl::write(const std::string &text) { tic_.print(text); }

void Wrl::writel(const std::string &text) {
  tic_.print(text);
  tic_.put_char('\n');
}

void Wrl::writep(const std::string &text, const std::string &postfix) {
  tic_.print(text);
  tic_.print(postfix);
}

bool Wrl::input(const std::string &prefix) {
  if (!is_inputing) {
    buffer_.clear();
    writep(prefix, " ");
    is_inputing = true;
  }

  const char c = tic_.get_char();
  if (c < 0) {
    return false;
  }

  if (c == 127) {
    if (!buffer_.empty()) {
      buffer_.pop_back();
      tic_.back();
    }
  } else if (c == '\n') {
    is_inputing = false;
  } else {
    buffer_.push_back(c);
    tic_.put_char(c);
  }

  if (!is_inputing) {
    write("\n");
  }

  return !is_inputing;
}

bool Wrl::input() { return input(prefix_); }

} // namespace jhmnn
