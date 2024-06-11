#pragma once

#include <wrl/tic.hpp>

#include <string>

namespace jhmnn {

class Wrl {

public:
  Wrl(std::string &buffer);
  Wrl(std::string &buffer, const std::string &prefix);
  ~Wrl();

  void init();
  void finalize();

  void cur_left();
  void cur_right();
  void cur_begin();
  void cur_end();

  void write(const std::string &text);
  void writel(const std::string &text);
  void writep(const std::string &text, const std::string &postfix);
  bool input(const std::string &prefix);
  bool input();

private:
  Tic tic_{};

  std::string prefix_;
  std::string &buffer_;

  std::size_t cur_pos_;

  bool is_inputing = false;
};

} // namespace jhmnn
