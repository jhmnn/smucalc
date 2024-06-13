#include <wrl/wrl.hpp>

#include <cstdarg>
#include <cstdio>

namespace jhmnn {

Wrl::Wrl(std::string &buffer) : buffer_(buffer), cur_pos_(buffer.size()) {}
Wrl::Wrl(std::string &buffer, const std::string &prefix)
    : prefix_(prefix), buffer_(buffer), cur_pos_(buffer.size()) {}
Wrl::~Wrl() { finalize(); }

void Wrl::init() { tic_.init(); }

void Wrl::finalize() { tic_.finalize(); }

void Wrl::cur_left() {
  if (cur_pos_ > 0) {
    tic_.cur_set_x(--cur_pos_);
  }
}

void Wrl::cur_right() {
  if (cur_pos_ < buffer_.size()) {
    tic_.cur_set_x(++cur_pos_);
  }
}

void Wrl::cur_begin() { tic_.cur_set_x(cur_pos_ = 0); }

void Wrl::cur_end() { tic_.cur_set_x(cur_pos_ = buffer_.size()); }

void Wrl::write(const std::string &text) { tic_.print(text); }

void Wrl::writel(const std::string &text) {
  tic_.print(text);
  tic_.put_char('\n');
}

void Wrl::writep(const std::string &text, const std::string &postfix) {
  tic_.print(text);
  tic_.print(postfix);
}

void Wrl::writee(const std::string &text) {
  tic_.cur_save();
  tic_.cur_set_x(buffer_.size() + prefix_.size() + 2);
  tic_.print(text);
  tic_.cur_load();
}

void Wrl::writef(const char *format, ...) {
  tic_.cur_save();
  tic_.cur_set_x(buffer_.size() + prefix_.size() + 2);
  va_list args;
  va_start(args, format);
  std::vprintf(format, args);
  va_end(args);
  tic_.cur_load();
}

bool Wrl::input(const std::string &prefix) {
  if (!is_inputing) {
    buffer_.clear();
    writep(prefix, " ");
    is_inputing = true;
  }

  tic_.cur_set_x(cur_pos_ + prefix_.size() + 2);
  const std::string s = tic_.read();

  if (s.substr(0, 3) == "\033[D") {
    cur_left();
  } else if (s.substr(0, 3) == "\033[C") {
    cur_right();
  } else if (s[0] == 127) {
    if (cur_pos_ > 0) {
      buffer_.erase(--cur_pos_, 1);
    }
  } else if (s[0] == '\n') {
    is_inputing = false;
  } else if (s[0] != '\033') {
    buffer_.insert(buffer_.begin() + cur_pos_++, s[0]);
  }

  tic_.clear_line();
  writep(prefix, " " + buffer_);
  tic_.cur_set_x(cur_pos_ + prefix_.size() + 2);

  if (!is_inputing) {
    cur_begin();
  }

  return !is_inputing;
}

bool Wrl::input() { return input(prefix_); }

} // namespace jhmnn
