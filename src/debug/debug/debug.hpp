#pragma once

namespace jhmnn {

class Debug {
public:
  static void log(const char *format, ...);
  static void clear();

public:
  static bool is_debug;
};

} // namespace jhmnn
