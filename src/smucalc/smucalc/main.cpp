#include <wrl/wrl.hpp>

int main() {
  std::string buffer;
  jhmnn::Wrl wrl(buffer, ":");
  wrl.init();

  while (true) {
    if (wrl.input()) {
      if (buffer == "exit") {
        break;
      }
    }
  }

  wrl.finalize();

  return 0;
}
