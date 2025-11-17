#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "Statement.hpp"

constexpr int RECORDER_END_LINE = 2000000000;
class Recorder {
 public:
  ~Recorder();

  void add(int line, Statement* stmt);
  void remove(int line);
  const Statement* get(int line) const noexcept;
  bool hasLine(int line) const noexcept;
  void clear() noexcept;
  void printLines() const;
  int nextLine(int line) const noexcept;

 private:
  // TODO.
  std::map<int, Statement*> lines;
};
