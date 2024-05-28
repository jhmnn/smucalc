#pragma once

#include <string>
#include <termios.h>

namespace jhmnn {

class Tic {
public:
  Tic();

  void init();
  void finalize();

  void clear_line() const;
  void back() const;

  void print(const std::string &s) const;
  void put_char(char c) const;
  int get_char();

private:
  termios term_attrs_{};
  std::string read_buf_;
};

} // namespace jhmnn
