#include <wrl/tic.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <unistd.h>

namespace jhmnn {

Tic::Tic() { read_buf_.resize(8, '\0'); }

void Tic::init() {
  // std::printf("\033[?25l");
  tcgetattr(0, &term_attrs_);

  termios tattr{};
  std::memcpy(&tattr, &term_attrs_, sizeof(tattr));

  tattr.c_lflag &= ~ICANON;
  tattr.c_lflag &= ~ECHO;
  tattr.c_lflag &= ISIG;

  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;

  tcsetattr(0, TCSAFLUSH, &tattr);
}

void Tic::finalize() {
  // std::printf("\033[?25h");
  tcsetattr(0, TCSAFLUSH, &term_attrs_);
}

void Tic::clear_line() const {
  printf("\033[0G");
  char const clear_code[] = "\033[2K";
  static_cast<void>(::write(1, clear_code, sizeof(clear_code) - 1) >= 0);
}

void Tic::back() const { print("\b \b"); }

void Tic::cur_move_x(int offset) const {
  if (offset < 0) {
    printf("\033[%dD", -offset);
  } else {
    printf("\033[%dC", offset);
  }
}

void Tic::cur_set_x(std::size_t x) const { printf("\033[%luG", x); }

void Tic::cur_save() const { print("\033[s"); }

void Tic::cur_load() const { print("\033[u"); }

void Tic::set_bg_color(Color color) const {
  const int n = 40 + static_cast<int>(color);
  printf("\033[%dm", n);
}

void Tic::set_fg_color(Color color) const {
  const int n = 30 + static_cast<int>(color);
  printf("\033[0;%dm", n);
}

void Tic::reset_color() const { printf("\033[0m"); }

void Tic::print(const std::string &s) const { printf("%s", s.c_str()); }

void Tic::printf(const char *format, ...) const {
  va_list args;
  va_start(args, format);
  std::vprintf(format, args);
  va_end(args);
  static_cast<void>(std::fflush(stdout));
}

void Tic::put_char(char c) const { printf("%c", c); }

int Tic::get_char() {
  const int n = ::read(0, read_buf_.data(), read_buf_.size() - 1);
  if (read_buf_[0] == '\033' || n < 0) {
    return -1;
  }

  return read_buf_[0];
}

std::string Tic::read() {
  ::read(0, read_buf_.data(), read_buf_.size() - 1);
  return read_buf_;
}

} // namespace jhmnn
