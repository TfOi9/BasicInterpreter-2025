#include "../include/Recorder.hpp"
#include <utility>

// TODO: Imply interfaces declared in the Recorder.hpp.

void Recorder::add(int line, std::unique_ptr<Statement> stmt) {
    lines[line] = std::move(stmt);
}

void Recorder::remove(int line) {
    if (lines.count(line)) {
        lines.erase(line);
    }
}

const Statement* Recorder::get(int line) const noexcept {
    if (lines.count(line)) {
        auto it = lines.find(line);
        return it->second.get();
    }
    else {
        return nullptr;
    }
}

bool Recorder::hasLine(int line) const noexcept {
    return lines.count(line);
}

void Recorder::clear() noexcept {
    lines.clear();
}

Recorder::~Recorder() {
    clear();
}

void Recorder::printLines() const {
    for (auto it = lines.begin(); it != lines.end(); it++) {
        if (it->second != nullptr) {
            std::cout << it->second->text() << std::endl;
        }
    }
}

int Recorder::nextLine(int line) const noexcept {
    auto it = lines.upper_bound(line);
    if (it == lines.end()) {
        return RECORDER_END_LINE;
    }
    else {
        return it->first;
    }
}