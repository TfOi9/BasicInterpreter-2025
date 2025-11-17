#include "../include/VarState.hpp"

#include <iostream>
#include <algorithm>

#include "../include/utils/Error.hpp"

VarState::VarState() {
  dep = 0;
  values_.push_back(std::unordered_map<std::string, int>());
}

void VarState::indent() {
  dep++;
  values_.push_back(std::unordered_map<std::string, int>());
}

void VarState::dedent() {
  if (dep) {
    values_.pop_back();
    dep--;
  }
  else {
    throw BasicError("SCOPE UNDERFLOW");
  }
}

void VarState::setValue(const std::string& name, int value) {
  values_[dep][name] = value;
}

int VarState::getValue(const std::string& name) const {
  for (int i = dep; i >= 0; i--) {
    auto it = values_[i].find(name);
    if (it != values_[i].end()) {
      return it->second;
    }
  }
  throw BasicError("VARIABLE NOT DEFINED");
}

void VarState::clear() {
  values_.clear();
  dep = 0;
  values_.push_back(std::unordered_map<std::string, int>());
}
