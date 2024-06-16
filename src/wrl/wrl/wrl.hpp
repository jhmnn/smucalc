#pragma once

#include <wrl/history.hpp>
#include <wrl/tic.hpp>

#include <string>

namespace jhmnn {

class Wrl {
public:
  Wrl();
  Wrl(const std::string &prefix);
  ~Wrl();

  void init();
  void finalize();

  void cur_left();
  void cur_right();
  void cur_begin();
  void cur_end();

  void set_bg_color(Color color) const;
  void set_fg_color(Color color) const;
  void reset_color() const;

  void write(const std::string &text);
  void writel(const std::string &text);
  void writep(const std::string &text, const std::string &postfix);
  void writee(const std::string &text);
  void writef(const char *format, ...);
  bool input(const std::string &prefix);
  bool input();

  std::string &buffer();

private:
  void new_line();

private:
  Tic tic_;
  History hist_;

  std::string prefix_;

  std::string *buffer_;

  std::size_t cur_pos_;

  bool is_inputting = false;
};

} // namespace jhmnn
