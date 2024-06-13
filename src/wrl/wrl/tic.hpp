#pragma once

#include <string>
#include <termios.h>

namespace jhmnn {

enum class Color { Black, Red, Green, Yellow, Blue, Purple, Cyan, White };

class Tic {
public:
  Tic();

  void init();
  void finalize();

  void clear_line() const;
  void back() const;

  void cur_move_x(int offset) const;
  void cur_set_x(std::size_t x) const;
  void cur_save() const;
  void cur_load() const;

  void set_bg_color(Color color) const;
  void set_fg_color(Color color) const;
  void reset_color() const;

  void print(const std::string &s) const;
  void printf(const char *format, ...) const;
  void put_char(char c) const;
  int get_char();
  std::string read();

private:
  termios term_attrs_{};
  std::string read_buf_;
};

} // namespace jhmnn
