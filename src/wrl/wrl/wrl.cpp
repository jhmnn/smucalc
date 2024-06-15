#include <wrl/wrl.hpp>

#include <cstdarg>
#include <cstdio>

namespace jhmnn {

Wrl::Wrl()
    : hist_("./res/history"), buffer_(hist_.next()), cur_pos_(buffer_->size()) {
}

Wrl::Wrl(const std::string &prefix)
    : hist_("./res/history"), prefix_(prefix), buffer_(hist_.next()),
      cur_pos_(buffer_->size()) {}

Wrl::~Wrl() { finalize(); }

void Wrl::init() { tic_.init(); }

void Wrl::finalize() { tic_.finalize(); }

void Wrl::cur_left() {
  if (cur_pos_ > 0) {
    tic_.cur_set_x(--cur_pos_);
  }
}

void Wrl::cur_right() {
  if (cur_pos_ < buffer_->size()) {
    tic_.cur_set_x(++cur_pos_);
  }
}

void Wrl::cur_begin() { tic_.cur_set_x(cur_pos_ = 0); }

void Wrl::cur_end() { tic_.cur_set_x(cur_pos_ = buffer_->size()); }

void Wrl::set_bg_color(Color color) const { tic_.set_bg_color(color); }

void Wrl::set_fg_color(Color color) const { tic_.set_fg_color(color); }

void Wrl::reset_color() const { tic_.reset_color(); }

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
  tic_.cur_set_x(buffer_->size() + prefix_.size() + 1);
  tic_.print(text);
  tic_.cur_load();
}

void Wrl::writef(const char *format, ...) {
  tic_.cur_save();
  tic_.cur_set_x(buffer_->size() + prefix_.size() + 1);
  va_list args;
  va_start(args, format);
  std::vprintf(format, args);
  va_end(args);
  tic_.cur_load();
  static_cast<void>(std::fflush(stdout));
}

bool Wrl::input(const std::string &prefix) {
  if (!is_inputting) {
    buffer_ = hist_.next();
    buffer_->clear();
    is_inputting = true;
    write(prefix);
  }

  const std::string s = tic_.read();

  if (s.substr(0, 3) == "\033[A") {
    buffer_ = hist_.prev();
    cur_pos_ = buffer_->size();
  } else if (s.substr(0, 3) == "\033[B") {
    buffer_ = hist_.next();
    cur_pos_ = buffer_->size();
  } else if (s.substr(0, 3) == "\033[D") {
    cur_left();
  } else if (s.substr(0, 3) == "\033[C") {
    cur_right();
  } else if (s[0] == 127) {
    if (cur_pos_ > 0) {
      hist_.start_edit_curr_entry();
      buffer_->erase(--cur_pos_, 1);
    }
  } else if (s[0] == '\n') {
    is_inputting = false;
  } else if (s[0] != '\033') {
    hist_.start_edit_curr_entry();
    buffer_->insert(buffer_->begin() + cur_pos_++, s[0]);
  }

  tic_.clear_line();
  writep(prefix, *buffer_);
  tic_.cur_set_x(prefix.size() + cur_pos_ + 1);

  if (!is_inputting) {
    hist_.append(*buffer_);
    buffer_ = hist_.prev();
    cur_begin();
  }

  return !is_inputting;
}

bool Wrl::input() { return input(prefix_); }

std::string &Wrl::buffer() { return *buffer_; }

} // namespace jhmnn
