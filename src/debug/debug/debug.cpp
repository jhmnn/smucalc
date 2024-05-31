#include <debug/debug.hpp>

#include <cstdarg>
#include <cstdio>

namespace jhmnn {

bool Debug::is_debug = true;

void Debug::log(const char *format, ...) {
  if (!is_debug) {
    return;
  }

  std::FILE *out = std::fopen("log.txt", "a");
  va_list args;
  va_start(args, format);
  static_cast<void>(std::vfprintf(out, format, args));
  va_end(args);
  static_cast<void>(std::fclose(out));
}

void Debug::clear() {
  std::FILE *out = std::fopen("log.txt", "w");
  static_cast<void>(std::fclose(out));
}

} // namespace jhmnn
