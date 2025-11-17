#include "../include/Program.hpp"
#include "../include/utils/Error.hpp"

// TODO: Imply interfaces declared in the Program.hpp.
Program::Program() {
    programCounter_ = -1;
    programEnd_ = 0;
    jumped_ = 0;
}

void Program::addStmt(int line, Statement* stmt) {
    recorder_.add(line, stmt);
}

void Program::removeStmt(int line) {
    recorder_.remove(line);
}

void Program::list() const {
    recorder_.printLines();
}

void Program::clear() {
    recorder_.clear();
    vars_.clear();
    programCounter_ = -1;
    programEnd_ = 0;
    jumped_ = 0;
}

void Program::execute(Statement *stmt) {
    stmt->execute(vars_, *this);
}

int Program::getPC() const noexcept {
    return programCounter_;
}

void Program::changePC(int line) {
    programCounter_ = line;
    jumped_ = 1;
}

void Program::programEnd() {
    programEnd_ = 1;
}

bool Program::hasLine(int line) const {
    return recorder_.hasLine(line);
}

void Program::resetAfterRun() noexcept {
    programCounter_ = -1;
    programEnd_ = 0;
    jumped_ = 0;
}

void Program::run() {
    while (!programEnd_) {
        int toLine = recorder_.nextLine(programCounter_);
        if (jumped_) {
            jumped_ = 0;
            toLine = programCounter_;
        }
        if (toLine == RECORDER_END_LINE) {
            programEnd_ = 1;
        }
        else {
            programCounter_ = toLine;
            const Statement* stmt = recorder_.get(programCounter_);
            try {
                stmt->execute(vars_, *this);
            }
            catch (const BasicError& e) {
                std::cout << e.message() << "\n";
            }
        }
    }
    resetAfterRun();
}