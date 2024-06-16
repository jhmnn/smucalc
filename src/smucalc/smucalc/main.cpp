#include <calc/calc.hpp>
#include <debug/debug.hpp>
#include <lexer/lexer.hpp>
#include <wrl/wrl.hpp>

#include <iostream>



int main() {
  jhmnn::Debug::init();

  jhmnn::Wrl wrl(": ");
  wrl.init();

  jhmnn::Lexer lexer;
  jhmnn::Calc calc;
  double result = 0;
  std::string error;

  while (true) {
    const bool is_line_typed = wrl.input();
    auto &buffer = wrl.buffer();
    if (is_line_typed) {
      if (buffer == "exit") {
        wrl.write("\n");
        break;
      }

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

      if (!buffer.empty() && lexer.more()) {
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
