#include <wrl/history.hpp>

#include <debug/debug.hpp>

#include <filesystem>

#include <unistd.h>
#include <pwd.h>

std::filesystem::path get_home_dir() {
    if (const char* home = std::getenv("HOME")) {
        return home;
    }
    
    if (struct passwd* pw = getpwuid(getuid())) {
        return pw->pw_dir;
    }
    
    throw std::runtime_error("Unable to determine home directory");
}

namespace jhmnn {

History::History() : History(get_home_dir() / ".cache/smucalc/history") {}

History::History(const std::string &path) : path_(path), curr_entry_(0) {
  load();
}

History::~History() { save(); }

void History::save() {
  std::fstream f(path_, std::ios_base::app);
  const auto size = entries_.size();
  const auto begin = size > max_size_ ? size - max_size_ : 0;
  for (auto i = begin; i < size; ++i) {
    f << entries_[i].curr << '\n';
  }
  f.close();
}

void History::load() {
  std::fstream f(path_, std::ios_base::in);
  if (!f.is_open()) {
    f.open(path_, std::ios_base::out);
    return;
  }

  while (!f.fail()) {
    std::string s;
    char c = 0;
    while (!f.fail() && (c = f.get()) != '\n') {
      s += c;
    }
    entries_.push_back(Entry(std::move(s)));
  }

  entries_.pop_back();
  curr_entry_ = entries_.size();

  f.close();
}

void History::append(const std::string &s) {
  std::string tmp = s;

  if (curr_entry_ < entries_.size()) {
    auto &entry = entries_[curr_entry_];
    if (entry.is_edited) {
      entry.curr = entry.buff;
      entry.is_edited = false;
    }
  }

  if (!tmp.empty() && tmp != entries_.back().curr) {
    entries_.push_back(Entry(std::move(tmp)));
  }

  curr_entry_ = entries_.size();
}

void History::clear() { entries_.clear(); }

std::string *History::prev() {
  if (entries_.empty()) {
    return &buffer_;
  }

  if (curr_entry_ == 0) {
    return &entries_.front().curr;
  }

  return &entries_[--curr_entry_].curr;
}

std::string *History::next() {
  if (entries_.empty()) {
    return &buffer_;
  }

  if (curr_entry_ + 1 == entries_.size()) {
    ++curr_entry_;
    return &buffer_;
  }

  if (curr_entry_ + 1 > entries_.size()) {
    return &buffer_;
  }

  return &entries_[++curr_entry_].curr;
}

void History::start_edit_curr_entry() {
  if (curr_entry_ < entries_.size()) {
    auto &entry = entries_[curr_entry_];
    if (!entry.is_edited) {
      entry.buff = entry.curr;
      entry.is_edited = true;
    }
  }
}

} // namespace jhmnn
