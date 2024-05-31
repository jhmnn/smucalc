#include <calc/calc.hpp>
#include <debug/debug.hpp>
#include <lexer/lexer.hpp>
#include <wrl/wrl.hpp>

#include <iostream>

int main() {
  jhmnn::Debug::clear();

  std::string buffer;
  jhmnn::Wrl wrl(buffer, ":");
  wrl.init();

  jhmnn::Lexer lexer;
  jhmnn::Calc calc;
  while (true) {
    if (wrl.input()) {
      lexer.parse(buffer);
      std::cout << "> " << calc.solve(lexer) << '\n';
      if (buffer == "exit") {
        break;
      }
    }
  }

  wrl.finalize();

  return 0;
}
