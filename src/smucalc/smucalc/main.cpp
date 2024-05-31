#include <debug/debug.hpp>
#include <lexer/lexer.hpp>
#include <syntaxer/syntaxer.hpp>
#include <wrl/wrl.hpp>

#include <iostream>

int main() {
  jhmnn::Debug::clear();

  std::string buffer;
  jhmnn::Wrl wrl(buffer, ":");
  wrl.init();

  jhmnn::Lexer lexer;
  jhmnn::Syntaxer syntaxer;
  while (true) {
    if (wrl.input()) {
      lexer.parse(buffer);
      std::cout << "> " << syntaxer.calc_expr(lexer) << '\n';
      if (buffer == "exit") {
        break;
      }
    }
  }

  wrl.finalize();

  return 0;
}
