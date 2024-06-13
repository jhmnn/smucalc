#include <calc/calc.hpp>
#include <debug/debug.hpp>
#include <lexer/lexer.hpp>
#include <wrl/wrl.hpp>

#include <iostream>

void delete_spaces(std::string &s) {
  const auto begin = s.begin() + s.find_first_not_of(' ');
  const auto end = s.begin() + s.find_last_not_of(' ');
  s.erase(end + 1, s.end());
  s.erase(s.begin(), begin);
}

bool is_spaces_only(const std::string &s) {
  return s.find_first_not_of(' ') == std::string::npos;
}

int main() {
  jhmnn::Debug::init();

  std::string buffer;
  jhmnn::Wrl wrl(buffer, ":");
  wrl.init();

  jhmnn::Lexer lexer;
  jhmnn::Calc calc;
  double result = 0;
  std::string error;

  while (true) {
    if (wrl.input()) {
      if (buffer.empty() || is_spaces_only(buffer)) {
        wrl.write("\n");
        continue;
      }

      if (buffer == "exit") {
        wrl.write("\n");
        break;
      }

      delete_spaces(buffer);
      if (calc.is_correct()) {
        wrl.set_fg_color(jhmnn::Color::Blue);
        wrl.writef(" = %.15g", result);
        calc.save_vars();
      } else {
        wrl.set_fg_color(jhmnn::Color::Red);
        wrl.writef(" - %s", error.c_str());
        error.clear();
      }
      wrl.reset_color();
      wrl.write("\n");
    } else {
      lexer.parse(buffer);
      if (!buffer.empty() && !is_spaces_only(buffer)) {
        try {
          result = calc.solve(lexer);
          wrl.set_fg_color(jhmnn::Color::Green);
          wrl.writef("    (V) = %.15g", result);
        } catch (const std::exception &e) {
          error = e.what();
          wrl.set_fg_color(jhmnn::Color::Red);
          wrl.writef("    (X) - %s", error.c_str());
        }
      }
      wrl.reset_color();
    }
  }

  wrl.finalize();

  return 0;
}
