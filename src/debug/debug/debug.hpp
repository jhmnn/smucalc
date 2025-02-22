#pragma once

namespace jhmnn {

class Debug {
public:
  static void init();
  static void log(const char *format, ...);

public:
  static bool is_debug;
};

} // namespace jhmnn
