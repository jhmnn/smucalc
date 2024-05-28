#include <wrl/tic.hpp>

#include <cstdio>
#include <cstring>

#include <unistd.h>

namespace jhmnn {

Tic::Tic() { read_buf_.resize(8, '\0'); }

void Tic::init() {
  std::printf("\033[?25l");

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
  std::printf("\033[?25h");
  tcsetattr(0, TCSAFLUSH, &term_attrs_);
}

void Tic::clear_line() const {
  std::printf("\033[%dG", 0);
  char const clear_code[] = "\033[J";
  static_cast<void>(::write(1, clear_code, sizeof(clear_code) - 1) >= 0);
}

void Tic::back() const { print("\b \b"); }

void Tic::print(const std::string &s) const {
  std::printf("%s", s.c_str());
  static_cast<void>(std::fflush(stdout));
}

void Tic::put_char(char c) const {
  std::putchar(c);
  static_cast<void>(std::fflush(stdout));
}

int Tic::get_char() {
  const int n = read(0, read_buf_.data(), read_buf_.size() - 1);
  if (read_buf_[0] == '\033' || n < 0) {
    return -1;
  }

  return read_buf_[0];
}

} // namespace jhmnn
