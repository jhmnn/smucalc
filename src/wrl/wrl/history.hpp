#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace jhmnn {

class History {
private:
  struct Entry {
    Entry(const std::string &s) : curr(s), is_edited(false) {}
    Entry(std::string &&s) : curr(s), is_edited(false) {}

    std::string curr;
    std::string buff;

    bool is_edited;
  };

public:
  History();
  History(const std::string &path);
  ~History();

  void append(const std::string &str);
  void clear();

  std::string *prev();
  std::string *next();

  void start_edit_curr_entry();

private:
  void save();
  void load();

private:
  std::string path_;

  std::vector<Entry> entries_;
  std::size_t curr_entry_;

  std::string buffer_;

  const std::size_t max_size_ = 1000;
};

} // namespace jhmnn
